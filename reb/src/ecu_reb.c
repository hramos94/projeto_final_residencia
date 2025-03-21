#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <unistd.h>

uint8_t can_send_hazard_light(uint8_t status)
{
    struct can_frame frame = {.can_id = 0x400, .can_dlc = 8, .data = {0}};

    if (status)
    {
        frame.data[0] = 0x01;
    }
    else
    {
        frame.data[0] = 0x02;
    }

    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

uint8_t handle_tcu_can(unsigned char *data)
{
    unsigned char signalREB = data[0];
    if (signalREB == 0x01)
    {
        if (start_reb() == FAIL)
        {
            show_error("tcu_can.start_reb FAIL\n");
            return FAIL;
        }
    }

    if (signalREB == 0x02)
    {
        show_error("Deactivating REB.\n");
        if (cancel_reb() == FAIL)
        {
            // Need dennis task for naming
            show_error("tcu_can.cancel_reb FAIL\n");
            return FAIL;
        }
    }

    return SUCCESS;
}

uint8_t reb_can_send_ecu(uint8_t status)
{
    struct can_frame frame = {.can_id = REB_ECU_ID, .can_dlc = 8, .data = {0}};

    if (status == ECU_REB_START)
    {
        frame.data[0] = 0x01;
    }

    if (status == ECU_REB_CANCEL)
    {
        frame.data[0] = 0x02;
    }

    show_log("send can to ECU to stop vehicle");
    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

uint8_t reb_can_send_ipc(uint8_t status)
{
    struct can_frame frame = {.can_id = REB_IPC_ID, .can_dlc = 8, .data = {0}};

    if (status == IPC_REB_START)
    {
        frame.data[0] = 0x01;
    }

    if (status == IPC_REB_CANCEL)
    {
        frame.data[0] = 0x02;
    }

    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}
