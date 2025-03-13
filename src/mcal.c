#include <stdio.h>
#include <ecu.h>
#include <mcal.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

dIO pins[10];
// pin 0 - luz de pisca alerta
// pin 1 - botão do pisca alerta
// pin 2 - Server envia bloqueio REB
// pin 3 - reb envia sinal de REB ativado ao motorista
// pin 4 - Server envia desbloqueio REB
// pin 5 - REB envia sinal para motor ser bloqueado

pthread_t new_thread(void *func){
  pthread_t nthread;
  pthread_create(&nthread, NULL, func, NULL);  
  return nthread;
}

void show_error(char errorStr[]){
  printf("%s", errorStr);
}

uint8_t read_pint_status(uint8_t *p_pin, uint8_t *p_status){
  char *line = NULL;
  size_t len = 0;
  if(getline(&line, &len, stdin) == -1){
    return FAIL;
  }
  if (memcmp(line, "pin", 3) == 0) {
    int pin,  status;
    if(sscanf(line +4, "%d %d", &pin, &status) == 2){
      if(status == 0 || status == 1){
        *p_pin = pin;
        *p_status = status;
        return SUCCESS;
      }
    }

  }
  return FAIL;
}

uint8_t mcal_init(){

  pins[0].pinNumber = 0;
  pins[0].status = 0;

  pins[1].pinNumber = 1;
  pins[1].status = 0;

  return SUCCESS;
}

uint8_t read_pin_status(uint8_t *status, uint8_t pin){
  *status = pins[pin].status; 

  return SUCCESS;
}

uint8_t set_pin_status(uint8_t p_status, uint8_t p_pin){
  pins[p_pin].status = p_status;
  printf(">>> Set pin%d = %d\n",p_pin, p_status);
  return SUCCESS;
}

void go_sleep(uint8_t seconds){
  sleep(seconds);
}

// Function to verify if can socket exists 
uint8_t can_socket_verify(const char *interface)
{
    char verify_can_command[100];
    sprintf(verify_can_command,"ip link show %s > /dev/null 2>&1", interface);

    //return FAIL if interface doesn not exists
    if(system(verify_can_command) != 0) 
    {
      return FAIL;
    }
    return SUCCESS; 
}

// Function to verify if can socket is UP or DOWN
uint8_t can_socket_status(const char *interface)
{
    // Crete new CAN socket
    struct ifreq socket_info;
    int socket_descriptor;
    socket_descriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_descriptor < 0)
    {
        //perror("Error opening raw socket");
        return FAIL;
    }
    
    // Initialize the struct ifreq to hold the interface information
    strncpy(socket_info.ifr_name, interface, IFNAMSIZ);
    
    // check if the interface exist getting the status using ioctl 
    if (ioctl(socket_descriptor, SIOCGIFFLAGS, &socket_info) < 0) 
    {
        //perror("Error getting interface flags");
        close(socket_descriptor);
        return FAIL;
    }
    
    // Check if the interface is up - IFF_UP is a flag that indicate if 
    //interface is down
    if (socket_info.ifr_flags & IFF_UP)
    {
        close(socket_descriptor);
        return SUCCESS;
    } 
    else //interface is down
    {
        close(socket_descriptor);
        return FAIL;
    }

}





