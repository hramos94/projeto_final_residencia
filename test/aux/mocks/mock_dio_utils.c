#include "ecu.h"
#include "mcal.h"
#include "stdio.h"

int flag_fail_get_pin = 0;
int flag_fail_set_pin = 0;

int flag_cout_set_pin[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int flag_status_pin[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int count_success = 0;
int count = 0;

uint8_t dio_get_pin(uint8_t *status, uint8_t pin, dIO io[])
{
    if (flag_fail_get_pin == 1)
    {
        return FAIL;
    }

    if (flag_fail_get_pin == 2)
    {
        count_success++;
        if (count_success >= count)
        {
            return FAIL;
        }
    }
    if (flag_fail_get_pin == 3)
    {
        *status = 0x02U;
        return SUCCESS;
    }
    if (flag_fail_get_pin == 4)
    {
        *status = 0x01U;
        return SUCCESS;
    }

    *status = io[pin].status;
    return SUCCESS;
}
int set_pin_status_fail_counter = 0;

uint8_t dio_set_pin(uint8_t status, uint8_t pin, dIO io[])
{
    if (flag_fail_set_pin == 2)
    {
        if(set_pin_status_fail_counter == 0)
        {
            return FAIL;
        }
        else
        {
            set_pin_status_fail_counter --;
            return SUCCESS;
        }

    }
    if (flag_fail_set_pin == 1)
    {
        return FAIL;
    }
    io[pin].status = status;
    printf(">>> Set pin%d = %d\n", pin, status);

    if (status == 1)
    {
        flag_cout_set_pin[pin]++;
    }

    flag_status_pin[pin] = status;

    return SUCCESS;
}
