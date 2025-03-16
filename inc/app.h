#ifndef H_APP
#define H_APP

#include <stdint.h>

#define S_ON 0x01U
#define S_OFF 0x00U

uint8_t application_init();
uint8_t hazard_lights_blink();
uint8_t read_input();
uint8_t monitor_engine_block();
uint8_t monitor_read_can();

#endif
