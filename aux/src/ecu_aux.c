#include <ecu.h>
#include <ecu_aux.h>
#include <mcal.h>
#include <unistd.h>

uint8_t get_hazard_button_status(uint8_t *status)
{
    if (read_pin_status(status, 1) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t set_hazard_light(uint8_t status)
{
    if (set_pin_status(status, 0) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t get_tcu_start_reb(uint8_t *status)
{
    if (read_pin_status(status, 2) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t set_tcu_start_reb(uint8_t status)
{
    if (set_pin_status(status, 2) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t tcu_can_send_reb(uint8_t status)
{
    struct can_frame frame = {.can_id = TCU_REB_ID, .can_dlc = 8, .data = {0}};

    if (status == REB_START)
    {
        frame.data[0] = 0x01;
    }

    if (status == REB_CANCEL)
    {
        frame.data[0] = 0x02;
    }

    show_log("send can to REB to start REB");
    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

uint8_t handle_ecu_can(unsigned char *data)
{
    unsigned char signalREB = data[0];
    if (signalREB == 0x01)
    {
        if (block_engine() == FAIL)
        {
            show_error("bloc_engine FAIL\n");
            return FAIL;
        }
    }

    if (signalREB == 0x02)
    {
        if (unblock_engine() == FAIL)
        {
            show_error("unblock_engine FAIL\n");
            return FAIL;
        }
    }

    return SUCCESS;
}

uint8_t block_engine()
{

    show_log("block Engine Started");
    // Send command ON to pin 2 (hazard Light)
    if (set_pin_status(S_ON, 1) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (hazard Light)\n");
        return FAIL;
    }

    // Send command ON to pin 5 (block engine)
    if (set_pin_status(S_ON, 5) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (engine block)\n");
        return FAIL;
    }

    // Send command ON to pin 3 (driver notification)
    if (set_pin_status(S_ON, 3) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (driver notification)\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t unblock_engine()
{
    show_log("unblock engine started");
    // change pin 2 to off
    if (set_pin_status(S_OFF, 2) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (turn off block request)\n");
        return FAIL;
    }
    // Send unblock engine command (pin 5)
    if (set_pin_status(S_OFF, 5) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (engine unblock)\n");
        return FAIL;
    }

    // Remove driver notification (pin 3)
    if (set_pin_status(S_OFF, 3) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (remove driver notification)\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t handle_ipc_can(unsigned char *data)
{
    unsigned char signalREB = data[0];
    if (signalREB == 0x01)
    {
        if (set_reb_warning(S_ON) == FAIL)
        {
            show_error("bloc_engine FAIL\n");
            return FAIL;
        }
    }

    if (signalREB == 0x02)
    {
        if (set_reb_warning(S_OFF) == FAIL)
        {
            show_error("bloc_engine FAIL\n");
            return FAIL;
        }
    }

    return SUCCESS;
}

uint8_t set_reb_warning(uint8_t status)
{
    // Remove driver notification (pin 3)
    if (set_pin_status(status, 3) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL  (driver notification)\n");
        return FAIL;
    }
    return SUCCESS;
}
