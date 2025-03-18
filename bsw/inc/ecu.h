#ifndef H_ECU
#define H_ECU

#include <stdint.h>

#define SUCCESS 0
#define FAIL 1

// ON/OFF Definitions (modularize by context -> ecu)
#define S_ON 0x01U
#define S_OFF 0x00U

uint8_t read_console();

#endif
