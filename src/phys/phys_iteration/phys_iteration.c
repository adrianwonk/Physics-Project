#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <ncurses.h>

#include "../../log.h"
#include "../obj.h"
#include "../phys_physList/phys_physList.h"
#include "../../const.h"
#include "../../math/math.h"
#include "phys_force_updater.h"
#include "phys_gravity.h"

#define PI 3.1415927
#define PI_HALF 1.5707964
#define COL_MAX 5

// CURRENT ASSUMPTIONS:
// - no elasticity
// - objects move one at a time, in order of subscription



/* forward declarations
 * */
//----------------------------------------------------/
void phys_detect(struct physItem *, struct physList *, float);
void phys_collide(struct physItem *, struct physItem **, int);
/****************************************************/


/* phys iteration 
 * */
void phys_iterate(struct physList *pList, int topLeftX,
               int topLeftY, float delta){
    
    // variable declaration
    struct physItem *target;
    int num;

    // Extract first physItem, and size of list. 
    target = pList->head;
    num = pList->size;

    for (int i = 0; i < num; i++, target = target->next){

        // skip simulating disabled items
        if (! (target -> processFlag)) continue;

        // PHYSITEM CONSUMATION PIPELINE
        phys_gravity(target, delta);
        
        // 1. Calculate acceleration difference for this timeslice.
        phys_enforce(target, delta);
        
        // 2. Move and collide
        phys_detect(target, pList, delta); 

        // 3. Re-apply timed-forces that aren't expired
        phys_processTimedForce(target, delta);

    }
}




static inline bool check_velocity_too_short(float mag_vel, float radius, float mag_toTarget){
    return mag_vel + radius < mag_toTarget;
}

static inline bool check_obtuse(float theta){
    return theta >= PI_HALF;
}

// COLLISION DETECTION AND HANDLING ========================================
void phys_detect(struct physItem *origin, struct physList * pList, float delta){

    // VELOCITY VECTOR AND MAGNITUDE SETUP =================================================
    struct vectf vect_vel = origin->velocity; 
    float sqrsum_vel = ve_sumOfSquare(vect_vel); // x^2 + y^2 of velocity
    float mag_vel = root(sqrsum_vel); //magnitude of velocity
    // =====================================================================================

    // N CLOSEST COLLISION TARGETS STORAGE SETUP ===========================================
    struct physItem* targets[COL_MAX];
    float min_sqrsum=INT_MAX;
    int n_targets = 0;
    // =====================================================================================

    // BACKSTEPPING HELPERS ================================================================
    float theta_col , sine_col , dist_target_col , tangent_len_col, radius_col;
    struct vectf target_pos;
    // =====================================================================================

    struct physItem *target = pList->head;
    int num = pList->size;

    for (int i = 0; i < num && target != NULL; i++, target = target->next){

        if (target == origin) continue;

        struct vectf vect_toTarget = ve_sub(target->coords, origin->coords);
        float sqrsum_toTarget = ve_sumOfSquare(vect_toTarget);
        float mag_toTarget = root(sqrsum_toTarget);
        float radius = target->radius + origin->radius;

        if (check_velocity_too_short(mag_vel, radius, mag_toTarget)) continue;

        // CALC acute angle between vect_vel and vect_toTarget =======
            float dotProduct = ve_dot(vect_toTarget, vect_vel);
            float theta = ve_angle(sqrsum_toTarget, sqrsum_vel, dotProduct);
            float sine = sinf(theta);
            float tangent_len = sine * mag_toTarget;
        // ===========================================================    
        
        if (check_obtuse(theta)) continue;

        // third check, case of shorter magnitudeForce < magnitudeTarget
        // and distance is greater than radius
        if (sqrsum_vel < sqrsum_toTarget){
            if(ve_sumOfSquare(ve_sub(vect_toTarget,vect_vel)) >= sqr(radius))
                continue;
        }
        // case of longer magnitudeForce, shortest distance is longer than radius
        else if ( tangent_len >= radius) continue;
        
        // add to collision consideration
        if (sqrsum_toTarget < min_sqrsum){
            min_sqrsum = sqrsum_toTarget;
            n_targets = 1;
            targets[0] = target;
            theta_col = theta; 
            sine_col = sine;
            dist_target_col = mag_toTarget;
            tangent_len_col = tangent_len;
            radius_col = radius;
            target_pos = target->coords;
        }

        else if (sqrsum_toTarget == min_sqrsum){
            targets[n_targets++] = target;
        }
    //////////////////////////////////////////////////////////////
    }
    if (min_sqrsum != INT_MAX){
        // backstep to point of collision 
        struct vectf normal = (struct vectf) {-vect_vel.y, vect_vel.x};
        normal = ve_scale(normal, invSqr( sqr(normal.x) + sqr(normal.y) ) * tangent_len_col );

        float backtrack = root( sqr(radius_col) - sqr(tangent_len_col) );
        struct vectf reverse = ve_scale(vect_vel, -1);
        reverse = ve_scale(reverse, invSqr( sqr(reverse.x) + sqr(reverse.y) ) * backtrack );

        origin->coords = ve_add(ve_add(target_pos, normal) , reverse);

        phys_collide(origin, targets, n_targets);
        return;    
    } else {
        origin->coords = ve_add(origin->coords, ve_scale(origin->velocity, delta));
    }
}

void phys_collide(struct physItem *target, struct physItem **victimArr, int n){
    // conservation of momentum.

    //     total_mtm = ve_add(total_mtm, ve_scale(victimArr[i]->velocity, victimArr[i]->mass));
    // }

    // struct vectf col_vects[COL_MAX];
    // for (int i = 0; i < COL_MAX; i++){
    //     col_vects[i] = ve_add(victimArr[i]->coords, ve_scale(target->coords, -1));
    // }
    
    // // get unit vectors
    // struct vectf unit_col_vects[COL_MAX];
    // for (int i = 0; i < COL_MAX; i++){
    //     float sum_of_sqrs = sqr(col_vects[i].x) + sqr(col_vects[i].y);
    //     unit_col_vects[i] = ve_scale(victimArr[i]->coords, invSqr(sum_of_sqrs));
    // }

    // change velocity of target, and victims
    
}
// --COLLISION DETECTION AND HANDLING ****************************************
