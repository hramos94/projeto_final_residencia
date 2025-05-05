#include <file_utils.h>
#include <stdarg.h>
// cppcheck-suppress misra-c2012-21.6
#include <stdio.h>
// cppcheck-suppress misra-c2012-21.10
#include <time.h>

FILE *file_fopen(const char *filename, const char *modes) { return fopen(filename, modes); }

struct tm *file_localtime_r(const time_t *timer, struct tm *tp) { return localtime_r(timer, tp); }

int file_fclose(FILE *stream) { return fclose(stream); }

int file_fprintf(FILE *stream, const char *format, ...)
{
    // cppcheck-suppress misra-c2012-17.1
    va_list args;
    // cppcheck-suppress misra-c2012-17.1
    va_start(args, format);
    int result = vfprintf(stream, format, args);
    // cppcheck-suppress misra-c2012-17.1
    va_end(args);
    return result;
}
