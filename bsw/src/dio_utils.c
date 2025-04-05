#include "ecu.h"
#include "mcal.h"
#include "stdio.h"

uint8_t dio_get_pin(uint8_t *status, uint8_t pin, dIO io[])
{
    *status = io[pin].status;
    return SUCCESS;
}

uint8_t dio_set_pin(uint8_t status, uint8_t pin, dIO io[])
{
    io[pin].status = status;
    printf(">>> Set pin%d = %d\n", pin, status);
    return SUCCESS;
}
