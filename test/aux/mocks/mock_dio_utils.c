#include "ecu.h"
#include "mcal.h"
#include "stdio.h"

int flag_fail_get_pin = 0;
int flag_fail_set_pin = 0;

int flag_cout_set_pin[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int flag_status_pin[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t dio_get_pin(uint8_t *status, uint8_t pin, dIO io[])
{
    if (flag_fail_get_pin == 1)
    {
        return FAIL;
    }
    *status = io[pin].status;
    return SUCCESS;
}

uint8_t dio_set_pin(uint8_t status, uint8_t pin, dIO io[])
{
    if (flag_fail_set_pin == 1)
    {
        return FAIL;
    }
    io[pin].status = status;
    printf(">>> Set pin%d = %d\n", pin, status);

    if(status == 1){
        flag_cout_set_pin[pin]++;
    }

    flag_status_pin[pin] = status;

    return SUCCESS;
}
