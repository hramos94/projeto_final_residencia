#ifndef H_APP
#define H_APP

#include <stdint.h>

#define S_ON 0x01U
#define S_OFF 0x00U

uint8_t application_init(void);
uint8_t hazard_lights_blink(void);
uint8_t read_input(void);
uint8_t monitor_read_can(void);
uint8_t monitor_tcu(void);
uint8_t check_can_communication(void);

#endif
