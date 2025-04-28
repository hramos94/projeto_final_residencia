#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <file_utils.h>

FILE *file_fopen(const char *filename, const char *modes) { return fopen(filename, modes); }

struct tm *file_localtime_r(const time_t *timer, struct tm *tp) { return localtime_r(timer, tp); }

int file_fclose(FILE *stream) { return fclose(stream); }

int file_fprintf(FILE *stream, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfprintf(stream, format, args);
    va_end(args);
    return result;
}
