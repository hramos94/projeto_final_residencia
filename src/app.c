#include <app.h>
#include <ecu.h>
#include <mcal.h>

uint8_t application_init()
{

    if (mcal_init() == FAIL)
    {
        show_error("mcal_init FAIL\n");
    }

    return SUCCESS;
}

uint8_t read_input()
{
    while (1)
    {
        if (read_console() == FAIL)
        {
            show_error("app.read_console FAIL\n");
        }
    }
}

uint8_t hazard_lights_blink()
{
    while (1)
    {
        uint8_t status = 0;
        if (get_hazard_button_status(&status) == FAIL)
        {
            show_error("get_hazard_button_status FAIL\n");
            return FAIL;
        }
        if (status == S_ON)
        {
            if (set_hazard_light(S_ON) == FAIL)
            {
                show_error("set_hazard_light FAIL\n");
                return FAIL;
            };
            go_sleep(1);
            if (set_hazard_light(S_OFF) == FAIL)
            {
                show_error("set_hazard_light FAIL\n");
                return FAIL;
            };
            go_sleep(1);
        }
        else
        {
            go_sleep(2);
        }
    }
}

uint8_t monitor_engine_block()
{
    while (1)
    {
        if (block_engine() == FAIL)
        {
            show_error("app.block_engine FAIL\n");
        }
        go_sleep(1);
        if (unblock_engine() == FAIL)
        {
            show_error("app.unblock_engine FAIL\n");
        }
        go_sleep(1);
    }
}
