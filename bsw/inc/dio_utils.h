#ifndef H_DIO_UTILS
#define H_DIO_UTILS
#include "mcal.h"
#include <stdint.h>  // Para tipos como uint8_t

uint8_t dio_get_pin(uint8_t *status, uint8_t pin, dIO io[]);
uint8_t dio_set_pin(uint8_t status, uint8_t pin, dIO io[]);

#endif
