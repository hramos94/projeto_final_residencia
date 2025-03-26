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
 * @brief function that makes hazard lights blink
 * @return SUCCESS(0); FAIL(1).
 */
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
            // handle message from REB to ECU
            if (frame.can_id == REB_ECU_ID)
            {
                handle_ecu_can(frame.data);
            }

            // handle message from REB to IPC
            if (frame.can_id == REB_IPC_ID)
            {
                handle_ipc_can(frame.data);
            }
            if (frame.can_id == 0x015 && frame.data[0] == 0x10)
            {
                show_log("Comunicação CAN OK\n");
                reb_con = 1;
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
 * @brief Handle the Reb ON and Reb OFF Buttons from GUI
 * @return SUCCESS(0); FAIL(1).
 */
uint8_t monitor_tcu()
{
    while (1)
    {
        uint8_t status = 0;
        // get REB ON Button status
        if (get_tcu_start_reb(&status) == FAIL)
        {
            show_error("get_tcu_star_reb FAIL\n");
            return FAIL;
        }
        // Activate REB
        if (status == S_ON)
        {
            // Toggle OFF Button to not enter in this loop
            if (set_tcu_start_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_start_reb FAIL\n");
                return FAIL;
            }
            // Send frame CAN to REB UNIT to start REB
            if (tcu_can_send_reb(REB_START) == FAIL)
            {
                show_error("tcu_can_send_reb FAIL\n");
                return FAIL;
            }
        }

        // get REB OFF Button status
        if (get_tcu_cancel_reb(&status) == FAIL)
        {
            show_error("get_tcu_cancel_reb FAIL\n");
            return FAIL;
        }
        // Deactivate REB
        if (status == S_ON)
        {
            // Toggle OFF Button to not enter in this loop
            if (set_tcu_cancel_reb(S_OFF) == FAIL)
            {
                show_error("set_tcu_cancel_reb FAIL\n");
                return FAIL;
            }
            // Send fram CAN to REB UNIT to cancel REB
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

//@TODO - deixar isso no DEFINE
struct can_frame test_frame = {.can_id = 0x013, .can_dlc = 1, .data = {0xFA}};
struct can_frame response_frame = {.can_id = 0x015, .can_dlc = 1, .data = {0x10}};

//@TODO - deixar um comentário padrão, verificar se tem requisitos, colocar como @requir{XX_YYY}
uint8_t check_can_communication()
{
    while (1)
    {
        reb_con = 0;
        int cont = 0;

        if (can_send_vcan0(&test_frame) == FAIL)
        {
            show_error("Erro ao enviar mensagem de teste\n");
        }

        while (reb_con == 0)
        {
            if (cont >= 5)
            {
                set_pin_status(1, REB_IPC_FAULT_PIN);
                if (can_send_vcan0(&test_frame) == FAIL)
                {
                    show_error("Erro ao enviar mensagem de teste\n");
                }
                cont = 0;
            }
            go_sleep(1);
            cont++;
        }
        set_pin_status(0, REB_IPC_FAULT_PIN);
    }
}
