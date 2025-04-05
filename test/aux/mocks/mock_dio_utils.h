#ifndef H_MOCK_CAN_UTILS
#define H_MOCK_CAN_UTILS

#include "mcal.h"

uint8_t dio_get_pin(uint8_t *status, uint8_t pin, dIO io[]);
uint8_t dio_set_pin(uint8_t status, uint8_t pin, dIO io[]);
#endif
