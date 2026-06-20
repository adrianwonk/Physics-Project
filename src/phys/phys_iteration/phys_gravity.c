#include "../../math/math.h"
#include "../obj.h"

void phys_gravity(struct physItem *target, float delta){
    // bypasses force system cuz all objects have the same acceleration!
    target -> velocity = ve_add(target -> velocity, ve_scale(target -> gravity, delta)
    );
}
