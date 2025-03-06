#ifndef H_ECU
#define H_ECU

#include <stdint.h>

#define SUCCESS 0
#define FAIL 1

uint8_t get_hazard_button_status(uint8_t *status);
uint8_t set_hazard_light(uint8_t status);
uint8_t read_console();

#endif
