#include <ecu.h>
#include <mcal.h>
#include <unistd.h>

#include "dtc_codes_bsw.h"
#include "dtc_logger.h"

/**
 *  @brief function get by console the status and number of the PIN set it.
 *  @return SUCCESS(0), FAIL(1)
 */

uint8_t read_console(void)
{
    uint8_t pin = 0, status = 0;
    if (read_pint_status(&pin, &status) == FAIL)
    {
        REPORT_ERROR("read_pin_status FAIL\n", DTC_READ_PIN_FAIL);
        return FAIL;
    }

    if (set_pin_status(status, pin) == FAIL)
    {
        REPORT_ERROR("read_console.set_pin_status FAIL\n", DTC_READ_CONSOLE_FAIL);
        return FAIL;
    }

    return SUCCESS;
}
