#ifndef H_APP
#define H_APP

#include <stdint.h>

#define S_ON 0x01U
#define S_OFF 0x00U

#define REB_RUNNING 0x01U
#define REB_CANCELED 0x00U

#define CAN_TEST_ID 0x100
#define CAN_RESPONSE_ID 0x101

uint8_t application_init();
uint8_t hazard_lights_blink();
uint8_t read_input();
uint8_t monitor_read_can();
uint8_t monitor_tcu();
uint8_t start_reb();
uint8_t cancel_reb();
uint8_t respond_to_can_test();

#endif
