#pragma once
#include <stdbool.h>
#include "objects.h"

/* phys_detect logger
 *   One <id>.detect file per non-floor physItem. Each tick contains, per
 *   target physItem (excluding floor and self), the outcome of each of the
 *   three guards in phys_detect: 'P' passed, 'F' failed (rejected by that
 *   guard), '-' not evaluated because an earlier guard rejected.
 *
 *   Item count is assumed constant; classification (floor vs. not) is fixed
 *   at start time based on the item's initial processFlag.
 */

// Walk pList, classify items, open output files. Idempotent.
void phys_logger_start(struct physList *pList);

// Advance to a new time step. Call once per phys_iterate tick.
void phys_logger_tick(void);

// Bracket guard records belonging to one origin's phys_detect call.
void phys_logger_origin_begin(struct physItem *origin);
void phys_logger_origin_end(void);

// Record outcome of guard `g` (0..2) against `target`. `passed` means the
// guard's rejection condition was NOT met (control flow proceeded past it).
// Calls with floor or unknown targets are no-ops.
void phys_logger_guard(struct physItem *target, int g, bool passed);
