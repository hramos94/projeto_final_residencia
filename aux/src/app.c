#include <app.h>
#include <ecu.h>
#include <ecu_aux.h>
#include <mcal.h>

uint8_t application_init()
{

    if (mcal_init() == FAIL)
    {
        show_error("mcal_init FAIL\n");
    }

    if (can_init() == FAIL)
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

uint8_t monitor_read_can()
{
    while (1)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
            if (frame.can_id == REB_ECU_ID)
            {
                handle_ecu_can(frame.data);
            }

            if (frame.can_id == REB_IPC_ID)
            {
                handle_ipc_can(frame.data);
            }
        }
        else
        {
            show_error("Error monitor_read_can\n");
            go_sleep(2);
        }
    }
}

uint8_t monitor_tcu()
{
    while (1)
    {
        uint8_t status = 0;
        if (get_tcu_start_reb(&status) == FAIL)
        {
            show_error("get_tcu_star_reb FAIL\n");
            return FAIL;
        }
        if (status == S_ON)
        {
            if (set_tcu_start_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_start_reb FAIL\n");
                return FAIL;
            }
            if (tcu_can_send_reb(REB_START) == FAIL)
            {
                show_error("tcu_can_send_reb FAIL\n");
                return FAIL;
            }
        }

        if (get_tcu_cancel_reb(&status) == FAIL)
        {
            show_error("get_tcu_cancel_reb FAIL\n");
            return FAIL;
        }
        if (status == S_ON)
        {
            if (set_tcu_cancel_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_cancel_reb FAIL\n");
                return FAIL;
            }
            if (tcu_can_send_reb(REB_CANCEL) == FAIL)
            {
                show_error("tcu_can_send_reb FAIL\n");
                return FAIL;
            }
        }
        else
        {
            go_sleep(2);
        }
    }
}
