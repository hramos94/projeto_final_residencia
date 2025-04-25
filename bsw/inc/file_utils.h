#ifndef H_FILE_UTILS
#define H_FILE_UTILS

#include <stdio.h>
#include <time.h>

FILE *file_fopen(const char *filename, const char *modes);
struct tm *file_localtime_r(const time_t *timer, struct tm *tp);
int file_fprintf(FILE *stream, const char *format, ...);
int file_fclose(FILE *stream);
int file_fprintf(FILE *stream, const char *format, ...);

#endif
