#include <ecu.h>
#include <ecu_aux.h>
#include <mcal.h>
#include <pins.h>
#include <unistd.h>

uint8_t get_hazard_button_status(uint8_t *status)
{
    if (read_pin_status(status, HAZARD_BUTTON_PIN) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t set_hazard_light(uint8_t status)
{
    if (set_pin_status(status, HAZARD_LIGHTS_PIN) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t get_tcu_start_reb(uint8_t *status)
{
    if (read_pin_status(status, REB_ACTIVATE_PIN) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t set_tcu_start_reb(uint8_t status)
{
    if (set_pin_status(status, REB_ACTIVATE_PIN) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t set_tcu_cancel_reb(uint8_t status)
{
    if (set_pin_status(status, REB_DEACTIVATE) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t get_tcu_cancel_reb(uint8_t *status)
{
    if (read_pin_status(status, REB_DEACTIVATE) == FAIL)
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
        show_log("send can to REB to start REB");
        frame.data[0] = 0x01;
    }

    if (status == REB_CANCEL)
    {
        show_log("send can to REB to stop REB");
        frame.data[0] = 0x02;
    }

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
            show_error("block_engine FAIL\n");
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
    // Send command ON to pin 1 (hazard Light)
    if (set_pin_status(S_ON, HAZARD_BUTTON_PIN) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (hazard Light)\n");
        return FAIL;
    }

    // Send command ON to pin 5 (block engine)
    if (set_pin_status(S_ON, ENGINE_REB_MODE) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (engine block)\n");
        return FAIL;
    }

    // Send command ON to pin 3 (driver notification)
    if (set_pin_status(S_ON, REB_IPC_WARNING) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (driver notification)\n");
        return FAIL;
    }
    return SUCCESS;
}

uint8_t unblock_engine()
{
    show_log("unblock engine started");
    // change pin 1 to off
    if (set_pin_status(S_OFF, HAZARD_BUTTON_PIN) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (turn off block request)\n");
        return FAIL;
    }
    // Send unblock engine command (pin 5)
    if (set_pin_status(S_OFF, ENGINE_REB_MODE) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL (engine unblock)\n");
        return FAIL;
    }

    // Remove driver notification (pin 3)
    if (set_pin_status(S_OFF, REB_IPC_WARNING) == FAIL)
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
        show_log("Receive from REB ipc to start");
        if (set_reb_warning(S_ON) == FAIL)
        {
            show_error("bloc_engine FAIL\n");
            return FAIL;
        }
    }

    if (signalREB == 0x02)
    {
        show_log("Receive from REB ipc to cancel");
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
    if (set_pin_status(status, REB_IPC_WARNING) == FAIL)
    {
        show_error("ECU.set_pin_status FAIL  (driver notification)\n");
        return FAIL;
    }
    return SUCCESS;
}
