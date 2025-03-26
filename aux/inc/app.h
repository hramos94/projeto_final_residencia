#ifndef H_APP
#define H_APP

#include <stdint.h>

#define S_ON 0x01U
#define S_OFF 0x00U

#define CAN_TEST_ID     0x100  // ID da mensagem de teste
#define CAN_RESPONSE_ID 0x101  // ID da mensagem de resposta
#define CAN_TEST_TIMEOUT 5  // Tempo limite em segundos

uint8_t application_init();
uint8_t hazard_lights_blink();
uint8_t read_input();
uint8_t monitor_engine_block();
uint8_t initiation_start_reb();
uint8_t monitor_read_can();
uint8_t monitor_tcu();
uint8_t check_can_communication();

#endif
