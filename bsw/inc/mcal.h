#ifndef H_MCAL
#define H_MCAL

#include <linux/can.h>     // struct can_frame, canid_t
#include <linux/can/raw.h> // CAN_RAW para socket
#include <pthread.h>
#include <stdint.h>

typedef struct
{
    uint8_t pinNumber;
    uint8_t status;
} dIO;

typedef struct
{
    canid_t can_id; /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    __u8 can_dlc;   /* frame payload length in byte (0 .. 8) */
    __u8 __pad;     /* padding */
    __u8 __res0;    /* reserved / padding */
    __u8 __res1;    /* reserved / padding */
    __u8 data[8] __attribute__((aligned(8)));
} can_frame;

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
uint8_t can_interface_status(int *can_socket, const char *interface);
uint8_t can_bind_socket(int *can_socket, const char *interface);
uint8_t can_send(int *can_socket, struct can_frame *frame);
uint8_t can_read(int *can_socket, struct can_frame *frame);

uint8_t can_send_vcan0(struct can_frame *frame);
uint8_t can_read_vcan0(struct can_frame *frame);

uint8_t can_init();
uint8_t can_close();

void show_log(char logStr[]);

#endif
