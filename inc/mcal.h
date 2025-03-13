#ifndef H_MCAL
#define H_MCAL

#include <stdint.h>
#include <pthread.h>

typedef struct {
  uint8_t pinNumber;
  uint8_t status;
}dIO;

void show_error(char errorStr[]);
uint8_t mcal_init();
uint8_t read_pin_status(uint8_t *status, uint8_t pin);
uint8_t set_pin_status(uint8_t p_status, uint8_t p_pin);
void go_sleep(uint8_t seconds);
pthread_t new_thread(void *func);
uint8_t endthread(int thread_numb);
uint8_t read_pint_status(uint8_t *p_pin, uint8_t *p_status);

uint8_t can_socket_open(int *can_socket);
uint8_t can_socket_close(int *can_socket);
uint8_t can_interface_verify(const char *interface);
uint8_t can_interface_status(const char *interface, int *can_socket);

#endif
