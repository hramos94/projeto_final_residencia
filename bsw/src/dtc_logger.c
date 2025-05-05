#include <stdint.h>
// cppcheck-suppress misra-c2012-21.6
#include <stdio.h>
// cppcheck-suppress misra-c2012-21.10
#include <time.h>

#include "file_utils.h"

#include "dtc_logger.h"

#define LOG_FILE "dtc_log.txt"

/**
 *  @brief Log a Diagnostic Trouble Code (DTC) to a local text file with timestamp.
 *
 *  This function appends the DTC identifier along with the current timestamp
 *  to the file defined in LOG_FILE. Each log entry represents a failure
 *  point detected by the REB system and can be used for
 *  diagnostics.
 *
 *  @param dtc_id The 32-bit identifier of the DTC to log.
 *
 *  @return None.
 *  @requir{SwHLR_F_16}
 */

void log_dtc(uint32_t dtc_id)
{
    int ret = 0;
    FILE *fp = file_fopen(LOG_FILE, "a");
    if (fp == NULL)
    {
        perror("log_dtc fopen failed.\n");
        ret = 1;
    }

    if (ret == 0)
    {
        time_t now = time(NULL);
        struct tm t;
        if (file_localtime_r(&now, &t) == NULL)
        {
            perror("log_dtc localtime_r failed.\n");
            ret = 1;
        }

        if (ret == 0)
        {

            if (file_fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] DTC 0x%08X\n", t.tm_year + 1900,
                             t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, dtc_id) < 0)
            {
                perror("log_dtc fprintf failed.\n");
            }
        }
        if (file_fclose(fp) != 0)
        {
            perror("log_dtc fclose failed.\n");
        }
    }
}
