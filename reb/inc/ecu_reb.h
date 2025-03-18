#ifndef H_ECU_REB
#define H_ECU_REB

#include <stdint.h>

uint8_t start_reb();
uint8_t block_engine();
uint8_t unblock_engine();
uint8_t engine_block_status(uint8_t *status);
uint8_t can_send_hazard_light(uint8_t status);
uint8_t handle_tcu_can(unsigned char data[]);

#endif
