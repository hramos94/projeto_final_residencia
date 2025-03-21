#ifndef H_ECU_REB
#define H_ECU_REB

#include <stdint.h>

#define REB_TIMEOUT 20

#define ECU_REB_START 1
#define ECU_REB_CANCEL 2

#define IPC_REB_START 1
#define IPC_REB_CANCEL 2

uint8_t can_send_hazard_light(uint8_t status);
uint8_t handle_tcu_can(unsigned char data[]);
uint8_t reb_can_send_ecu(uint8_t status);
uint8_t reb_can_send_ipc(uint8_t status);

#endif
