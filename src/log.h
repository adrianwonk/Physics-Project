#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include "phys/phys.h"

void log_append(const char *fmt, ...);
void log_item(const char *prec, struct physItem *target, float delta);

#endif
