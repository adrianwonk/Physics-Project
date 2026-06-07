#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include "phys/phys.h"

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

void log_item(const char *prefix, struct physItem *target, float delta){
    log_append("%-10.9s delta: %.9f secs, vel: {% -5.1f, % -5.1f}, F: {% -5.1f, % -5.1f}, coords: {% -5.1f, % -5.1f}\n"
        , prefix
        , delta * INT_MAX
        , target->velocity.x
        , target->velocity.y
        , target->F_total.x
        , target->F_total.y
        , target->coords.x
        , target->coords.y
        
        );

}
