#pragma once
#include <stdbool.h>
#include "../math/math.h"
#include "obj.h"
#include "phys_physList.h"

/* physics stuff*/
    void phys_iterate(struct physList *, int, int, float);
    void pi_timedForce(struct physItem *target, float sec, struct vectf force );
/****************************************************/


