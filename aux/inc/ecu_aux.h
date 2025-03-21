#ifndef H_ECU_AUX
#define H_ECU_AUX

#include <stdint.h>

#define REB_START 1
#define REB_CANCEL 2

uint8_t get_hazard_button_status(uint8_t *status);
uint8_t set_hazard_light(uint8_t status);
uint8_t get_tcu_start_reb(uint8_t *status);
uint8_t set_tcu_start_reb(uint8_t status);
uint8_t tcu_can_send_reb(uint8_t status);
uint8_t handle_ecu_can(unsigned char *data);
uint8_t block_engine();
uint8_t unblock_engine();
uint8_t handle_ipc_can(unsigned char *data);
uint8_t set_reb_warning(uint8_t status);
uint8_t set_tcu_cancel_reb(uint8_t status);
uint8_t get_tcu_cancel_reb(uint8_t *status);

#endif
