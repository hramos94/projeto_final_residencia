#ifndef DTC_LOGGER_H
#define DTC_LOGGER_H

#include <stdint.h>

void log_dtc(uint32_t dtc_id);

// Macro to swap show_error() to show_error() AND log_dtc().
// Reduces lines usage.
#define REPORT_ERROR(msg, code)                                                                    \
    do                                                                                             \
    {                                                                                              \
        log_dtc(code);                                                                             \
        show_error(msg);                                                                           \
    } while (0)

#endif
