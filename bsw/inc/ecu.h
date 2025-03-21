#ifndef H_ECU
#define H_ECU

#include <stdint.h>

#define SUCCESS 0
#define FAIL 1

#define TCU_REB_ID 0x150
#define REB_ECU_ID 0x200
#define REB_IPC_ID 0x300

// ON/OFF Definitions (modularize by context -> ecu)
#define S_ON 0x01U
#define S_OFF 0x00U

#define SHOW_LOG 1

uint8_t read_console();

#endif
