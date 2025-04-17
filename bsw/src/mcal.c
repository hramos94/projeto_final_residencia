#include "can_utils.h"
#include "dio_utils.h"
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

dIO pins[IOPINS];
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

/**
 *
 *  @brief Create a new thread in posix.
 *  @param func Pointer to the function that will runin the thread.
 *  @return The created pthread_t identifier.
 */
pthread_t new_thread(void *func)
{
    pthread_t nthread;
    pthread_create(&nthread, NULL, func, NULL);
    return nthread;
}

/**
 *  @brief Show in terminal ERROR Message.
 *  @param errorStr Pointer to the charr array of message.
 *  @requir{SwHLR_F_16}
 */
void show_error(char errorStr[]) { printf("%s", errorStr); }

/**
 *  @brief Set the status of PIN by terminal
 *
 *  @param p_pin Pointer of PIN number to be read.
 *  @param p_ststus Pointer to store the read status of the PIN (0 or 1).
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_13}
 */
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
                free(line);
                return SUCCESS;
            }
        }
    }
    free(line);
    return FAIL;
}

/**
 *  @brief Inicializate the PINs status.
 *
 *  @return SUCCESS(0), FAIL(1)
 */
uint8_t mcal_init(void)
{

    for (int i = 0; i < IOPINS; i++)
    {
        pins[i].pinNumber = i;
        pins[i].status = 0;
    }

    return SUCCESS;
}

/**
 *  @brief Get the status of desired PIN number.
 *
 *  @param status Pointer to store the read status of the PIN (0 or 1).
 *  @param pin Pointer of PIN number to be read.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_13}
 */
uint8_t read_pin_status(uint8_t *status, uint8_t pin)
{
    uint8_t return_status = SUCCESS;
    if (pin < IOPINS)
    {
        return_status = dio_get_pin(status, pin, pins);
    }
    else
    {
        return_status = FAIL;
    }
    return return_status;
}

/**
 *  @brief Set the status of desired PIN number.
 *
 *  @param status Pointer to store the status of the PIN (0 or 1).
 *  @param pin Pointer of PIN number to be read.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_13}
 */
uint8_t set_pin_status(uint8_t p_status, uint8_t p_pin)
{
    uint8_t status = SUCCESS;
    if (p_pin < IOPINS)
    {
        status = dio_set_pin(p_status, p_pin, pins);
    }
    else
    {
        status = FAIL;
    }
    return status;
}

/**
 *  @brief Sleep thread POSIX.
 *
 *  @param seconds How many seconds to sleep.
 */
void go_sleep(uint8_t seconds) { sleep(seconds); }

//======================================================================
//======================== CAN =========================================
//======================================================================

/**
 *  @brief Opens Can socket - does not depend on VCAN interface.
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_socket_open(int *can_socket)
{
    uint8_t status = SUCCESS;
    if ((*can_socket = socket_create(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket Open Failed: ");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief Closes CAN socket - does not depend on VCAN interface.
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_socket_close(int *can_socket)
{
    uint8_t status = SUCCESS;
    if (socket_close(*can_socket) < 0)
    {
        perror("Socket Close Failed:");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief Function to verify if can interface is UP or DOWN.
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @param interface Pointer to char interface
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_interface_status(int *can_socket, const char *interface)
{
    uint8_t status = SUCCESS;
    struct ifreq socket_info; // Initialize the struct ifreq to hold the interface information
    strncpy(socket_info.ifr_name, interface, IFNAMSIZ);

    // check if the interface exist getting the status using ioctl
    if (can_ioctl(*can_socket, SIOCGIFFLAGS, &socket_info) < 0)
    {
        perror("Error getting interface flags");
        status = FAIL;
    }

    // Check if the interface is up - IFF_UP is a flag that indicate if interface is UP
    if (socket_info.ifr_flags & IFF_UP && status == SUCCESS)
    {
        status = SUCCESS;
    }
    else
    {
        status = FAIL;
    }
    return status;
}

/**
 *  @brief Armed with the interface index, we can now bind the socket to the CAN Interface:
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @param interface Pointer to char interface
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_bind_socket(int *can_socket, const char *interface)
{
    uint8_t status = SUCCESS;
    struct ifreq ifr;
    strcpy(ifr.ifr_name, interface);
    can_ioctl(*can_socket, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (can_bind(*can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief function to send frames on CAN BUS.
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @param frame Pointer to frame that will be send to can.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_send(int *can_socket, struct can_frame *frame)
{
    uint8_t status = SUCCESS;
    if (can_write(can_socket, frame) == FAIL)
    {
        perror("Can Write error: ");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief function to read frames on CAN - this will block execution until a frame is received.
 *
 *  @param can_socket Pointer to store the number of socket.
 *  @param frame Pointer to frame that will be read from can.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_read(int *can_socket, struct can_frame *frame)
{
    uint8_t status = SUCCESS;
    // this will block until frame avaliable
    int nbytes = can_read_socket(*can_socket, frame, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        perror("Can Read Error: ");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief function that open SOCKET CAN.
 *
 *  @param my_vcan Pointer to store the number of socket.
 *  @param interface Pointer to store the char interface.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_start(int *my_vcan, const char *interface)
{
    uint8_t status = SUCCESS;
    if (can_socket_open(my_vcan) == FAIL)
    {
        perror("Can socket open Error: ");
        status = FAIL;
    }
    if ((can_interface_status(my_vcan, interface) == FAIL) && (status == SUCCESS)) 
    {
        perror("Can interface Error: ");
        status = FAIL;
    }
    if ((can_bind_socket(my_vcan, interface) == FAIL) && (status == SUCCESS))
    {
        perror("Can bind Error: ");
        status = FAIL;
    }
    return status;
}

/**
 *  @brief function that send frame CAN to vcan0 of LINUX
 *
 *  @param frame Pointer of frame can that will be send.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_send_vcan0(struct can_frame *frame) { return can_send(&my_vcan, frame); }

/**
 *  @brief function that will read frame can coming from vcan0 of LINUX
 *
 *  @param frame Pointer of frame can that will be send.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_read_vcan0(struct can_frame *frame) { return can_read(&my_vcan, frame); }

/**
 *  @brief function that initialize Socket CAN Linux.
 *
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_init(void) { return can_start(&my_vcan, interface); }

/**
 *  @brief function that close Socket CAN Linux.
 *
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_9}
 *  @requir{SwHLR_F_6}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_15}
 */
uint8_t can_close(void) { return can_socket_close(&my_vcan); }

/**
 *  @brief Show in terminal LOG Message.
 *  @param errorStr Pointer to the charr array of message.
 *  @requir{SwHLR_F_16}
 */
void show_log(char logStr[])
{
    if (SHOW_LOG == 1)
    {
        printf("%s\n", logStr);
    }
}
