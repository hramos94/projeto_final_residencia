#include <stdarg.h>
#include <stdio.h>
#include <time.h>

int flag_fail_FILE = 0;

FILE *file_fopen(const char *filename, const char *modes)
{
    if (flag_fail_FILE == 0)
    {
        return fopen(filename, modes);
    }
    else
    {
        return NULL;
    }
}

struct tm *file_localtime_r(const time_t *timer, struct tm *tp)
{
    if (flag_fail_FILE == 0)
    {
        return localtime_r(timer, tp);
    }
    else
    {
        return NULL;
    }
}

int file_fclose(FILE *stream)
{
    if (flag_fail_FILE == 0)
    {
        return fclose(stream);
    }
    else
    {
        return -1;
    }
}

int file_fprintf(FILE *stream, const char *format, ...)
{
    if (flag_fail_FILE == 0)
    {
        va_list args;
        va_start(args, format);
        int result = vfprintf(stream, format, args);
        va_end(args);
        return result;
    }
    else
    {
        return -1;
    }
}
