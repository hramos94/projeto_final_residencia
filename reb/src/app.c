#include <app.h>
#include <ecu.h>
#include <ecu_reb.h>
#include <mcal.h>
#include <time.h>

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

uint8_t monitor_read_can()
{
    while (1)
    {

        struct can_frame frame = {
            .can_id = 29, .can_dlc = 8, .data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        if (can_read_vcan0(&frame) != FAIL)
        {
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

uint8_t start_reb()
{
    clock_t start_time, current_time;
    double elapsed_time = 0;

    // start the counting to 5 min
    start_time = clock();

    if (reb_can_send_ipc(IPC_REB_START) == FAIL)
    {
        show_error("start_reb.reb_can_send_ipc FAIL\n");
        return FAIL;
    }

    while (1)
    {
        current_time = clock();
        elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

        if (elapsed_time >= REB_TIMEOUT)
        {
            reb_can_send_ecu(ECU_REB_START);
            return SUCCESS;
        }

        // TODO
        if (1 == 0)
        {
            // TODO cancelando a contagem
            return SUCCESS;
        }
    }

    return SUCCESS;
}
