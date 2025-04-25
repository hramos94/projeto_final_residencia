#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <pins.h>
// cppcheck-suppress misra-c2012-21.10
#include <time.h>

#include "dtc_codes_reb.h"
#include "dtc_logger.h"

uint8_t flag_reb_canceled = REB_RUNNING;

/**
 * @brief Initializes the application by configuring drivers.
 * @return SUCCESS(0); FAIL(1).
 */
uint8_t application_init(void)
{
    uint8_t status = SUCCESS;
    mcal_init();

    if (can_init() == FAIL)
    {
        REPORT_ERROR("can_init FAIL\n", DTC_CAN_INIT_FAIL);
        status = FAIL;
    }
    return status;
}

/**
 * @brief Handle terminal PIN inputs
 *
 * @note This function handle set pins by terminal
 * ex: "pin 1 0"  set the pin number 1 to status 0
 *
 */
uint8_t read_input(void)
{
    while (1)
    {
        if (read_console() == FAIL)
        {
            REPORT_ERROR("app.read_console FAIL\n", DTC_READ_INPUT_FAIL);
        }
    }
}

/**
 * @brief Handle messages received from CAN BUS
 * @return SUCCESS(0); FAIL(1).
 * @requir{SwHLR_F_1}
 * @requir{SwHLR_F_3}
 * @requir{SwHLR_F_6}
 * @requir{SwHLR_F_10}
 * @requir{SwHLR_F_13}
 * @requir{SwHLR_F_15}
 */
uint8_t monitor_read_can(void)
{
    uint8_t status = SUCCESS;
    while (status == SUCCESS)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
            // Handle message received from Telematic Control Unit(TCU)
            if (frame.can_id == TCU_REB_ID)
            {
                if (handle_tcu_can(frame.data) == FAIL)
                {
                    show_error("handle_tcu_can FAIL\n\n");
                }
            }
            if ((frame.can_id == AUX_COM_ID) && (frame.data[0] == AUX_COM_SIG))
            {
                // Check REB x AUX Communication and send response as ok
                struct can_frame response;
                    response.can_id = REB_COM_ID;
                    response.can_dlc = 1;
                    response.data[0] = REB_COM_SIG;

                if (can_send_vcan0(&response) == FAIL)
                {
                    REPORT_ERROR("respond_to_can_test: Error to send response\n",
                                 DTC_CAN_RESPONSE_FAIL);

                    status = FAIL;
                }
            }
        }
        else
        {
            REPORT_ERROR("Error monitor_read_can\n", DTC_MONITOR_READ_CAN_FAIL);
            go_sleep(2);
        }
    }
    return status;
}

/**
 * @brief Handle the deactivating REB
 * @return SUCCESS(0); FAIL(1).
 * @requir{SwHLR_F_8}
 */
uint8_t cancel_reb(void)
{
    uint8_t status = SUCCESS;
    flag_reb_canceled = REB_CANCELED;
    //  Send by CAN to IPC the Caceled REB status
    if (reb_can_send_ipc(IPC_REB_CANCEL) == FAIL)
    {
        REPORT_ERROR("cancel_reb.reb_can_send_ipc FAIL\n", DTC_REB_CAN_IPC_CANCEL_FAIL);
        status = FAIL;
    }

    if (status == SUCCESS)
    {
        // Send by CAN to Engine Control Unit the Caceled REB status
        if (reb_can_send_ecu(ECU_REB_CANCEL) == FAIL)
        {
            REPORT_ERROR("cancel_reb.reb_can_send_ecu FAIL\n", DTC_REB_CAN_ECU_CANCEL_FAIL);
            status = FAIL;
        }
    }
    return status;
}

/**
 * @brief Handle the activating REB
 * @return SUCCESS(0); FAIL(1).
 * @requir{SwHLR_F_8}
 * @requir{SwHLR_F_12}
 * @requir{SwHLR_F_14}
 * @requir{SwHLR_F_2}
 */
uint8_t start_reb(void)
{
    uint8_t status = SUCCESS;

    // Reset flag when start reb
    flag_reb_canceled = REB_RUNNING;

    show_log("Start REB counting and send can to IPC");
    // send by CAN to IPC the start REB counting
    if (reb_can_send_ipc(IPC_REB_START) == FAIL)
    {
        REPORT_ERROR("start_reb.reb_can_send_ipc FAIL\n", DTC_REB_CAN_IPC_START_FAIL);
        status = FAIL;
    }

    return status;
}

/**
 * @brief Handle the activating REB countdown
 * @requir{SysHLR_3}
 * @requir{SwHLR_F_12}
 * @requir{SwHLR_F_14}
 * @requir{SwHLR_F_1}
 */
uint8_t countdown_reb(void)
{
    while (1)
    {
        struct timespec start_time;
        struct timespec current_time;
        uint8_t reb_countdown_active = 0;

        (void)clock_gettime(CLOCK_MONOTONIC, &start_time);

        // Verify the status of the pin
        if (read_pin_status(&reb_countdown_active, REB_COUNTDOWN_PIN) == FAIL)
        {
            show_error("read_pin_status ERROR\n");
        }

        while (reb_countdown_active != 0U)
        {
            double elapsed_time = 0.0;
            (void)clock_gettime(CLOCK_MONOTONIC, &current_time);

            elapsed_time = (current_time.tv_sec - start_time.tv_sec) +
                           ((double)(current_time.tv_nsec - start_time.tv_nsec) / 1e9);

            if (elapsed_time >= (double)REB_TIMEOUT)
            {
                if (reb_can_send_ecu(ECU_REB_START) == FAIL)
                {
                    show_error("reb_can_send_ecu ERROR\n");
                }
                
                if (set_pin_status(S_OFF, REB_COUNTDOWN_PIN) == FAIL)
                {
                    show_error("set_pin_status ERROR\n");
                }
            }

            // Verify the status of the pin again
            if (read_pin_status(&reb_countdown_active, REB_COUNTDOWN_PIN)== FAIL)
            {
                show_error("read_pin_status ERROR\n");
            }
        }

        go_sleep(1);
    }
}
