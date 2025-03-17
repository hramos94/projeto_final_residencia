#include <app.h>
#include <ecu.h>
#include <mcal.h>

uint8_t application_init()
{

    if (mcal_init() == FAIL)
    {
        show_error("mcal_init FAIL\n");
    }

    if(can_init() == FAIL)
    {
        show_error("can_init FAIL\n");
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


uint8_t send_can_hazard_light()
{
    while (1)
    {
        // TESTE; precisa abstrair
        uint8_t status;
        if (read_pin_status(&status, 9) == FAIL)
        {
            return FAIL;
        }

        if (status == 1)
        {
            if (set_pin_status(0, 9) == FAIL)
            {
                return FAIL;
            }
            if(can_send_hazard_light(1) == FAIL)
            {
                show_error("app.can_send_hazard FAIL\n");
                return FAIL;
            }
        }

        if (read_pin_status(&status, 8) == FAIL)
        {
            return FAIL;
        }

        if (status == 1)
        {
            if (set_pin_status(0, 8) == FAIL)
            {
                return FAIL;
            }
            if(can_send_hazard_light(0) == FAIL)
            {
                show_error("app.can_send_hazard FAIL\n");
                return FAIL;
            }
        }

        go_sleep(1);
    }
}

uint8_t monitor_read_can()
{
    while (1)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
            if (frame.can_id == 0x7E0)
            {
                handle_tcu_can(frame.data);
            }
        }
        else
        {
            show_error("Error monitor_read_can\n");
            go_sleep(2);
        }
    }
}
