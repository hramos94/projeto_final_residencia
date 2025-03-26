#include <ecu.h>
#include <mcal.h>
#include <unistd.h>

/**
 *  @brief function get by console the status and number of the PIN set it.
 *  @return SUCCESS(0), FAIL(1)
 */

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
