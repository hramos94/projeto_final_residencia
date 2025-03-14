#ifndef H_ECU
#define H_ECU

#include <stdint.h>

#define SUCCESS 0
#define FAIL 1

// ON/OFF Definitions (modularize by context -> ecu)
#define S_ON  0x01U
#define S_OFF 0x00U

uint8_t get_hazard_button_status(uint8_t *status);
uint8_t set_hazard_light(uint8_t status);
uint8_t read_console();
uint8_t block_engine();
uint8_t unblock_engine();
uint8_t engine_block_status(uint8_t *status);

#endif
