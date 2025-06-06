#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <pins.h>
#include <unistd.h>

#include "dtc_codes_reb.h"
#include "dtc_logger.h"

/**
 *  @brief Handle TCU signal received from can
 *
 *  @param data Pointer to frame message receive from TCU Can.
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_3}
 */
uint8_t handle_tcu_can(const unsigned char *data)
{
    uint8_t status = SUCCESS;

    // Get first byte.
    unsigned char signalREB = data[0];
    // Signal from TCU to cancel REB
    if (signalREB == (uint8_t)0x02U)
    {
        // Deactivate pin for countdown
        if (set_pin_status(S_OFF, REB_COUNTDOWN_PIN) == FAIL)
            {
                show_error("set_pin_status ERROR\n");
            }
        
        if (cancel_reb() == FAIL)
        {
            REPORT_ERROR("tcu_can.cancel_reb FAIL\n", DTC_TCU_CANCEL_REB_FAIL);
            status = FAIL;
        }
    }

    // Signal from TCU to start REB
    if (signalREB == (uint8_t)0x01U)
    {
        // Activate pin for countdown
        if (set_pin_status(S_ON, REB_COUNTDOWN_PIN) == FAIL)
        {
            show_error("set_pin_status ERROR\n");
        }

        if (start_reb() == FAIL)
        {
            REPORT_ERROR("tcu_can.start_reb FAIL\n", DTC_TCU_START_REB_FAIL);
            status = FAIL;
        }
    }
    return status;
}

/**
 *  @brief Send a can message to ECM to block or unblock engine.
 *
 *  @param status 1 to send frame data 0x01(Block);  2 to send frame data 0x02(Unblock).
 *  @return SUCCESS(0), FAIL(1)
 *  @requir{SwHLR_F_12}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_2}
 *  @requir{SwHLR_F_1}
 */
uint8_t reb_can_send_ecu(uint8_t status)
{
    uint8_t statusReturn = SUCCESS;

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
        statusReturn = FAIL;
    }

    return statusReturn;
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
    uint8_t statusReturn = SUCCESS;

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
        statusReturn = FAIL;
    }
    return statusReturn;
}
