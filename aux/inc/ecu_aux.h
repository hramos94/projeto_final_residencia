#ifndef H_ECU_AUX
#define H_ECU_AUX

#include <stdint.h>

uint8_t get_hazard_button_status(uint8_t *status);
uint8_t set_hazard_light(uint8_t status);

#endif
