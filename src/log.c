#include <stdio.h>
#include <stdarg.h>
#include "log.h"

void log_append(const char *fmt, ...) {
    FILE *f = fopen("hi", "a");
    if (f) {
        va_list args;
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);
        fclose(f);
    }
}
