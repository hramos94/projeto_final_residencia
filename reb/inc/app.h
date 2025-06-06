#ifndef H_APP
#define H_APP

#include <stdint.h>

#define S_ON 0x01U
#define S_OFF 0x00U

#define REB_RUNNING 0x01U
#define REB_CANCELED 0x00U

extern uint8_t flag_reb_canceled;

uint8_t application_init(void);
uint8_t read_input(void);
uint8_t monitor_read_can(void);
uint8_t start_reb(void);
uint8_t cancel_reb(void);
uint8_t countdown_reb(void);

#endif
