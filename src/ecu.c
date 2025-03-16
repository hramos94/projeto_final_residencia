#include <ecu.h>
#include <mcal.h>
#include <time.h>
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

uint8_t read_console()
{
    uint8_t pin = 0, status = 0;
    if (read_pint_status(&pin, &status) == FAIL)
    {
        show_error("read_pin_status FAIL\n");
        return FAIL;
    }

    if (set_pin_status(status, pin) == FAIL)
    {
        show_error("read_console.set_pin_status FAIL\n");
        return FAIL;
    }

    return SUCCESS;
}

uint8_t start_reb()
{
    uint8_t status = 0;
    struct timespec start_time, current_time;
    const int countdown_sec = 5;

    if (read_pin_status(&status, 3) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL (get start_reb status)\n");
        return FAIL;
    }

    if (status == S_ON)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        do
        {
            if (read_pin_status(&status, 3) == FAIL)
            {
                show_error("ECU.read_pin_status FAIL (during count)\n");
                return FAIL;
            }

            if (status == S_OFF)
            {
                return SUCCESS;
            }

            clock_gettime(CLOCK_MONOTONIC, &current_time);
            double elapsed_time = (current_time.tv_sec - start_time.tv_sec) +
                                  (current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;

            if (elapsed_time >= countdown_sec)
            {
                return block_engine();
            }

        } while (1);
    }

    return SUCCESS;
}

uint8_t block_engine()
{
    uint8_t status = 0;

    // check server status (pin 2)
    if (read_pin_status(&status, 2) == FAIL)
    {
        show_error("ECU.read_pin_status (server block request)\n");
        return FAIL;
    }

    if (status == S_ON)
    {

        // change pin 4 to off (unblock request)
        if (set_pin_status(S_OFF, 4) == FAIL)
        {
            show_error("ECU.set_pin_status FAIL (turn off unblock server request)\n");
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
    }

    return SUCCESS;
}

uint8_t unblock_engine()
{
    uint8_t status = 0;

    // Check server status (pin 4)
    if (read_pin_status(&status, 4) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL (server unblock request)\n");
        return FAIL;
    }
    if (status == S_ON)
    {

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
    }

    return SUCCESS;
}

uint8_t engine_block_status(uint8_t *status)
{
    // read engine status (block=1 or unblocked=0)
    if (read_pin_status(status, 5) == FAIL)
    {
        show_error("ECU.read_pin_status FAIL (get engine status)\n");
        return FAIL;
    }

    return SUCCESS;
}