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

// Função para verificar se a interface vcan0 existe
uint8_t verify_can_socket(const char *interface)
{
    char verify_can_command[100];
    sprintf(verify_can_command,"ip link show %s > /dev/null 2>&1", interface);

    //return 1 if interface exists and 0 if not
    if(system(verify_can_command) != 0) 
    {
      return FAIL;
    }
    else
    {
      return SUCCESS;
    }
  
}
