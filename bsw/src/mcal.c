#include <ecu.h>
#include <mcal.h>

#include <linux/can.h>     // struct can_frame, canid_t
#include <linux/can/raw.h> // CAN_RAW para socket
#include <net/if.h>        // struct ifreq, IFNAMSIZ
#include <stdint.h>        // uint8_t
#include <stdio.h>         // printf, perror, sprintf
#include <stdlib.h>        // system
#include <string.h>        // strcpy, strncpy, memset
#include <sys/ioctl.h>     // ioctl, SIOCGIFFLAGS, SIOCGIFINDEX
#include <sys/socket.h>    // socket, bind
#include <sys/types.h>     // Tipos de dados do sistema
#include <unistd.h>        // close, write

dIO pins[10];
// pin 0 - luz de pisca alerta
// pin 1 - botão do pisca alerta
// pin 2 - Server envia bloqueio REB
// pin 3 - reb envia sinal de REB ativado ao motorista
// pin 4 - Server envia desbloqueio REB
// pin 5 - REB envia sinal para motor ser bloqueado
//
// pin 8 - Envia sinal na redCAN para desligar o pisca alerta
// pin 9 - Envia sinal na redCAN para ligar o pisca alerta

int my_vcan;
const char *interface = "vcan0";

pthread_t new_thread(void *func)
{
    pthread_t nthread;
    pthread_create(&nthread, NULL, func, NULL);
    return nthread;
}

void show_error(char errorStr[]) { printf("%s", errorStr); }

uint8_t read_pint_status(uint8_t *p_pin, uint8_t *p_status)
{
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) == -1)
    {
        return FAIL;
    }
    if (memcmp(line, "pin", 3) == 0)
    {
        int pin, status;
        if (sscanf(line + 4, "%d %d", &pin, &status) == 2)
        {
            if (status == 0 || status == 1)
            {
                *p_pin = pin;
                *p_status = status;
                return SUCCESS;
            }
        }
    }
    return FAIL;
}

uint8_t mcal_init()
{

    pins[0].pinNumber = 0;
    pins[0].status = 0;

    pins[1].pinNumber = 1;
    pins[1].status = 0;

    return SUCCESS;
}

uint8_t read_pin_status(uint8_t *status, uint8_t pin)
{
    *status = pins[pin].status;

    return SUCCESS;
}

uint8_t set_pin_status(uint8_t p_status, uint8_t p_pin)
{
    pins[p_pin].status = p_status;
    printf(">>> Set pin%d = %d\n", p_pin, p_status);
    return SUCCESS;
}

void go_sleep(uint8_t seconds) { sleep(seconds); }

//======================================================================
//======================== CAN =========================================
//======================================================================

// Opens Can socket - does not depend on VCAN interface
uint8_t can_socket_open(int *can_socket)
{
    if ((*can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket Open Failed: ");
        return FAIL;
    }
    return SUCCESS;
}

// Closes CAN socket - does not depend on VCAN interface
uint8_t can_socket_close(int *can_socket)
{
    if (close(*can_socket) < 0)
    {
        perror("Socket Close Failed:");
        return FAIL;
    }
    return SUCCESS;
}

// Function to verify if can socket exists
uint8_t can_interface_verify(const char *interface)
{
    char verify_can_command[100];
    sprintf(verify_can_command, "ip link show %s > /dev/null 2>&1", interface);

    // return FAIL if interface doesn not exists
    if (system(verify_can_command) != 0)
    {
        return FAIL;
    }
    return SUCCESS;
}

// Function to verify if can interface is UP or DOWN
uint8_t can_interface_status(int *can_socket, const char *interface)
{
    struct ifreq socket_info; // Initialize the struct ifreq to hold the interface information
    strncpy(socket_info.ifr_name, interface, IFNAMSIZ);

    // check if the interface exist getting the status using ioctl
    if (ioctl(*can_socket, SIOCGIFFLAGS, &socket_info) < 0)
    {
        perror("Error getting interface flags");
        return FAIL;
    }

    // Check if the interface is up - IFF_UP is a flag that indicate if interface is UP
    if (socket_info.ifr_flags & IFF_UP)
    {
        return SUCCESS;
    }
    else
    {
        return FAIL;
    }
}

// Armed with the interface index, we can now bind the socket to the CAN Interface:
uint8_t can_bind_socket(int *can_socket, const char *interface)
{
    struct ifreq ifr;
    strcpy(ifr.ifr_name, interface);
    ioctl(*can_socket, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(*can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return FAIL;
    }
    return SUCCESS;
}

// function to send frames on CAN BUS
uint8_t can_send(int *can_socket, struct can_frame *frame)
{
    if (write(*can_socket, frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Can Write error: ");
        return FAIL;
    }
    return SUCCESS;
}

// function to read frames on CAN - this will block execution until a frame is received
uint8_t can_read(int *can_socket, struct can_frame *frame)
{
    // this will block until frame avaliable
    int nbytes = read(*can_socket, frame, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        perror("Can Read Error: ");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t can_start(int *my_vcan, const char *interface)
{
    if (can_socket_open(my_vcan) == FAIL)
    {
        perror("Can socket open Error: ");
        return FAIL;
    }
    if (can_interface_status(my_vcan, interface) == FAIL)
    {
        perror("Can interface Error: ");
        return FAIL;
    }
    if (can_bind_socket(my_vcan, interface) == FAIL)
    {
        perror("Can bind Error: ");
        return FAIL;
    }
    return SUCCESS;
}

// send can frame in vcan0
uint8_t can_send_vcan0(struct can_frame *frame)
{
    if (can_send(&my_vcan, frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

// read frame in vcan0 - will block until frame is received
uint8_t can_read_vcan0(struct can_frame *frame)
{
    if (can_read(&my_vcan, frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

uint8_t can_init()
{
    if (can_start(&my_vcan, interface) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

uint8_t can_close()
{
    can_socket_close(&my_vcan);
    return SUCCESS;
}
void show_log(char logStr[])
{
    if (SHOW_LOG == 1)
    {
        printf("%s\n", logStr);
    }
}
