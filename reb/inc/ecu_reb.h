#ifndef H_ECU_REB
#define H_ECU_REB

#include <stdint.h>

#define REB_TIMEOUT 20

#define ECU_REB_START 0x01U
#define ECU_REB_CANCEL 0x02U

#define IPC_REB_START 0x01U
#define IPC_REB_CANCEL 0x02U

uint8_t handle_tcu_can(const unsigned char data[]);
uint8_t reb_can_send_ecu(uint8_t status);
uint8_t reb_can_send_ipc(uint8_t status);

#endif
