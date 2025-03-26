#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <unistd.h>

/**
 *  @brief Send a CAN Message to turn ON or OF the hazard light
 *
 *  @param status 0 to send frame data 0x01(ON);  2 to send frame data 0x02(OFF).
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_10}
 */
uint8_t can_send_hazard_light(uint8_t status)
{
    struct can_frame frame = {.can_id = 0x400, .can_dlc = 8, .data = {0}};

    if (status)
    {
        // Send TURN ON signal
        frame.data[0] = 0x01;
    }
    else
    {
        // Send TURN OFF signal
        frame.data[0] = 0x02;
    }

    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

/**
 *  @brief Handle TCU signal received from can
 *
 *  @param data Pointer to frame message receive from TCU Can.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_3}
 */
uint8_t handle_tcu_can(unsigned char *data)
{
    // Get first byte.
    unsigned char signalREB = data[0];

    // Signal from TCU to start REB
    if (signalREB == 0x01)
    {
        if (start_reb() == FAIL)
        {
            show_error("tcu_can.start_reb FAIL\n");
            return FAIL;
        }
    }

    // Signal from TCU to cancel REB
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

/**
 *  @brief Send a can message to ECM to block or unblock engine.
 *
 *  @param status 1 to send frame data 0x01(Block);  2 to send frame data 0x02(Unblock).
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_12}
 */
uint8_t reb_can_send_ecu(uint8_t status)
{
    struct can_frame frame = {.can_id = REB_ECU_ID, .can_dlc = 8, .data = {0}};

    if (status == ECU_REB_START)
    {
        show_log("send can to ECU to stop vehicle");
        // block engine signal
        frame.data[0] = 0x01;
    }

    if (status == ECU_REB_CANCEL)
    {
        show_log("send can to ECU to remove reb blocking");
        // unblock engine signal
        frame.data[0] = 0x02;
    }

    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}

/**
 *  @brief Send a can message to Instrument Paincel Control status of REB.
 *
 *  @param status 1 to send frame data 0x01(ON);  2 to send frame data 0x02(OFF).
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_8}
 */
uint8_t reb_can_send_ipc(uint8_t status)
{
    struct can_frame frame = {.can_id = REB_IPC_ID, .can_dlc = 8, .data = {0}};

    if (status == IPC_REB_START)
    {
        show_log("Send can to IPC to start reb");
        // Reb is activated
        frame.data[0] = 0x01;
    }

    if (status == IPC_REB_CANCEL)
    {
        show_log("Send can to IPC to stop reb");
        // Reb is deactivated
        frame.data[0] = 0x02;
    }

    if (can_send_vcan0(&frame) == FAIL)
    {
        return FAIL;
    }
    return SUCCESS;
}
