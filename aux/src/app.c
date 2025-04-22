#include <app.h>
#include <ecu.h>
#include <ecu_aux.h>
#include <mcal.h>
#include <pins.h>

uint8_t reb_con = 0;

/**
 * @brief Initializes the application by configuring drivers.
 * @return SUCCESS(0); FAIL(1).
 */
uint8_t application_init(void)
{

    uint8_t status = SUCCESS;
    if (mcal_init() == FAIL)
    {
        show_error("mcal_init FAIL\n");
        status = FAIL;
    }

    if ((status == SUCCESS) && (can_init() == FAIL))
    {
        show_error("can_init FAIL\n");
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
            show_error("app.read_console FAIL\n");
        }
    }
}

/**
 * @brief function that makes hazard lights blink
 * @return SUCCESS(0); FAIL(1).
 */
uint8_t hazard_lights_blink(void)
{
    uint8_t ret = SUCCESS;
    while (ret == SUCCESS)
    {
        uint8_t status = 0;
        if (get_hazard_button_status(&status) == FAIL)
        {
            show_error("get_hazard_button_status FAIL\n");
            ret = FAIL;
        }
        if ((ret == SUCCESS) && (status == S_ON))
        {
            if ((ret == SUCCESS) && (set_hazard_light(S_ON) == FAIL))
            {
                show_error("set_hazard_light FAIL\n");
                ret = FAIL;
            };
            go_sleep(1);
            if ((ret == SUCCESS) && (set_hazard_light(S_OFF) == FAIL))
            {
                show_error("set_hazard_light FAIL\n");
                ret = FAIL;
            };
            go_sleep(1);
        }
        else
        {
            go_sleep(2);
        }
    }
    return ret;
}

/**
 * @brief Handle messages received from CAN BUS
 * @return SUCCESS(0); FAIL(1).
 * @requir{SwHLR_F_6}
 * @requir{SwHLR_F_10}
 * @requir{SwHLR_F_15}
 */
uint8_t monitor_read_can(void)
{
    uint8_t ret = SUCCESS;
    while (ret == SUCCESS)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
            // handle message from REB to ECU
            if (frame.can_id == REB_ECU_ID)
            {
                if (handle_ecu_can(frame.data) == FAIL)
                {
                    show_error("Handle_ecu_can ERROR\n");
                }
            }

            // handle message from REB to IPC
            if (frame.can_id == REB_IPC_ID)
            {
                if (handle_ipc_can(frame.data) == FAIL)
                {
                    show_error("handle_ipc_can ERROR\n");
                }
            }
            if ((frame.can_id == 0x015) && (frame.data[0] == 0x10))
            {
                // Return as ok the communication between REB x AUX
                reb_con = 1;
            }
        }
        else
        {
            show_error("Error monitor_read_can\n");
            go_sleep(2);
        }
    }
    return ret;
}

/**
 * @brief Handle the Reb ON and Reb OFF Buttons from GUI
 * @return SUCCESS(0); FAIL(1).
 */
uint8_t monitor_tcu(void)
{
    uint8_t ret = SUCCESS;
    while (ret == SUCCESS)
    {
        uint8_t status = 0;
        // get REB ON Button status
        if (get_tcu_start_reb(&status) == FAIL)
        {
            show_error("get_tcu_star_reb FAIL\n");
            ret = FAIL;
        }
        // Activate REB
        if ((ret == SUCCESS) && (status == S_ON))
        {
            // Toggle OFF Button to not enter in this loop
            if (set_tcu_start_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_start_reb FAIL\n");
                ret = FAIL;
            }
            // Send frame CAN to REB UNIT to start REB
            if ((ret == SUCCESS) && (tcu_can_send_reb(REB_START) == FAIL))
            {
                show_error("tcu_can_send_reb FAIL\n");
                ret = FAIL;
            }
        }

        // get REB OFF Button status
        if ((ret == SUCCESS) && (get_tcu_cancel_reb(&status) == FAIL))
        {
            show_error("get_tcu_cancel_reb FAIL\n");
            ret = FAIL;
        }
        // Deactivate REB
        if ((ret == SUCCESS) && (status == S_ON))
        {
            // Toggle OFF Button to not enter in this loop
            if (set_tcu_cancel_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_cancel_reb FAIL\n");
                ret = FAIL;
            }
            // Send fram CAN to REB UNIT to cancel REB
            if ((ret == SUCCESS) && (tcu_can_send_reb(REB_CANCEL) == FAIL))
            {
                show_error("tcu_can_send_reb FAIL\n");
                ret = FAIL;
            }
        }
        else
        {
            go_sleep(2);
        }
    }
    return ret;
}

/**
 * @brief Check communication CAN between REB e AUX modules sending a frame and receving a response.
 * @return SUCCESS(0); FAIL(1).
 * @requir{SwHLR_F_13}
 * @requir{SwHLR_F_15}
 */
uint8_t check_can_communication(void)
{
    // Frames to check cSommunication CAN between REB e AUX modules
    struct can_frame test_frame = {0};
    test_frame.can_id = AUX_COM_ID;
    test_frame.can_dlc = 1;
    test_frame.data[0] = AUX_COM_SIG;

    while (1)
    {
        reb_con = 0;
        int cont_tries = 0;

        if (can_send_vcan0(&test_frame) == FAIL)
        {
            show_error("check_can_communication: Error to send communication test\n");
        }
        uint8_t current_ipc_fault_pin_status = 0x00U;

        while (reb_con == 0x00U)
        {
            if (cont_tries >= 10)
            {
                // Try 10 times the communication test {SwHLR_F_15}
                if (read_pin_status(&current_ipc_fault_pin_status, REB_IPC_FAULT_PIN) == FAIL)
                {
                    show_error("read_pin_status ERROR\n");
                }
                if (current_ipc_fault_pin_status != 0x01U)
                {
                    if (set_pin_status(1, REB_IPC_FAULT_PIN) == FAIL)
                    {
                        show_error("set_pin_status ERROR\n");
                    }
                }
                if (can_send_vcan0(&test_frame) == FAIL)
                {
                    show_error("check_can_communication: Timeout CAN communication\n");
                }
                cont_tries = 0;
            }
            // Verify communication each 11 seconds
            go_sleep(1);
            cont_tries++;
        }
        if (read_pin_status(&current_ipc_fault_pin_status, REB_IPC_FAULT_PIN) == FAIL)
        {
            show_error("read_pin_status ERROR\n");
        }
        if (current_ipc_fault_pin_status != 0x00U)
        {
            if (set_pin_status(0, REB_IPC_FAULT_PIN) == FAIL)
            {
                show_error("set_pin_status ERROR\n");
            }
        }
    }
    return SUCCESS;
}
