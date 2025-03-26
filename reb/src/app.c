#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <time.h>

uint8_t flag_reb_canceled = REB_RUNNING;

/**
 * @brief Initializes the application by configuring drivers.
 * @return SUCCESS(0); FAIL(1).
 */
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

/**
 * @brief Handle terminal PIN inputs
 *
 * @note This function handle set pins by terminal
 * ex: "pin 1 0"  set the pin number 1 to status 0
 *
 */
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

/**
 * @brief Handle messages received from CAN BUS
 * @return SUCCESS(0); FAIL(1).
 * @requir SwHLR_F_9, SwHLR_F_6, SwHLR_F_10, SwHLR_F_15.
 */
uint8_t monitor_read_can()
{
    while (1)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
            // Handle message received from Telematic Control Unit(TCU)
            if (frame.can_id == TCU_REB_ID)
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

/**
 * @brief Handle the deactivating REB
 * @return SUCCESS(0); FAIL(1).
 * @requir SwHLR_F_8.
 */
uint8_t cancel_reb()
{
    flag_reb_canceled = REB_CANCELED;
    printf("Valor do REB_CANCELED = %d\n", flag_reb_canceled);
    // Send by CAN to IPC the Caceled REB status
    if (reb_can_send_ipc(IPC_REB_CANCEL) == FAIL)
    {
        show_error("cancel_reb.reb_can_send_ipc FAIL\n");
        return FAIL;
    }

    // Send by CAN to Engine Control Unit the Caceled REB status
    if (reb_can_send_ecu(ECU_REB_CANCEL) == FAIL)
    {
        show_error("cancel_reb.reb_can_send_ecu FAIL\n");
        return FAIL;
    }

    show_error("REB deactivate with success.\n");
    return SUCCESS;
}

/**
 * @brief Handle the activating REB
 * @return SUCCESS(0); FAIL(1).
 * @requir SwHLR_F_8, SwHLR_F_12.
 */
uint8_t start_reb()
{

    // Reset flag when start reb
    flag_reb_canceled = REB_RUNNING;
    countdown_activate = 1;


    show_log("Start REB counting and send can to IPC");
    // send by CAN to IPC the start REB counting
    if (reb_can_send_ipc(IPC_REB_START) == FAIL)
    {
        show_error("start_reb.reb_can_send_ipc FAIL\n");
        return FAIL;
    }

    // TODO create another thread? probably a mutex?
       //printf("Reb canceled entrou %d==%d\n", REB_CANCELED, flag_reb_canceled);

        if (flag_reb_canceled == REB_CANCELED)
        {
            show_error("REB canceled before timeout\n");
            return SUCCESS;
        }

    return SUCCESS;
}
