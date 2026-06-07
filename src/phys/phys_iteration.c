#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#include "../log.h"
#include "obj.h"
#include "phys_physList.h"
#include "../const.h"
#include "../math/math.h"

#define PI 3.1415927
#define PI_HALF 1.5707964
#define COL_MAX 5

// CURRENT ASSUMPTIONS:
// - no elasticity
// - objects move one at a time, in order of subscription



/* forward declarations
 * */
//----------------------------------------------------/
void phys_gravity(struct physItem *, float);
void phys_detect(struct physItem *, struct physList *, float);
void phys_collide(struct physItem *, struct physItem **, int);
void phys_enforce(struct physItem *, float);
void phys_processTimedForce( struct physItem *target, float delta );
/****************************************************/


/* phys iteration 
 * */
void phys_iterate(struct physList *pList, int topLeftX,
               int topLeftY, float delta){
    struct physItem *target;
    int num;

    target = pList->head;
    num = pList->size;
    for (int i = 0; i < num; i++, target = target->next){
        if (! (target -> processFlag)) continue;

        // 0. Gravity bypass force system
        phys_gravity(target, delta);
        
        // 1. Process force, accumulate velocity
        phys_enforce(target, delta);
        
        // 2. Move and collide based on velocity
        phys_detect(target, pList, delta); 

        // 3. Process timed force applications
        phys_processTimedForce(target, delta);

    }
}

// apply a force vector (mass times acceleration) over a period of time.
void phys_enforce(struct physItem *target, float delta){
    float mass = target -> mass;

    /* reads F_total and resets it */
    pthread_mutex_lock(&(target->F_lock));
        struct vectf force = target -> F_total;
        target -> F_total = (struct vectf){0,0};
    pthread_mutex_unlock(&(target->F_lock));
    
    struct vectf accel = ve_scale(force, inv(mass));
    struct vectf scaled = ve_scale(accel, delta);
    target -> velocity = ve_add(target->velocity, scaled);
}

// Acknowledge delta for each timeBlock, re-queue unexpired forces.
void phys_processTimedForce( struct physItem *target, float delta ){
    struct timeBlock *prev = NULL; // prev ptr is either NULL, or previous timeBlock in iteration.
    struct timeBlock *curr = target->time_q;
    while ( curr != NULL ){
        // deplete remaining time by deltaTime
        curr->t_remain -= delta;

        // check timeBlock expiration
        if (curr -> t_remain <= 0.f){ 
            if (prev == NULL){ // delete head
                target -> time_q = curr -> next; 
                free(curr);
                curr = target -> time_q; 
                continue;
            } else { // delete body
                prev->next = curr->next;
                free(curr);
                curr = prev -> next;
                continue;
            }
        }

        pi_applyForce( target, curr->force );

        prev = curr;
        curr = curr->next;
    }
}

void phys_gravity(struct physItem *target, float delta){
    // bypasses force system cuz all objects have the same acceleration!
    target -> velocity = ve_add(target -> velocity, ve_scale(target -> gravity, delta)
    );
}

// COLLISION DETECTION AND HANDLING ========================================
void phys_detect(struct physItem *origin, struct physList * pList, float delta){
    /* 1. force applied to origin
     * 2. pre-magnitude 
     * 3.  magnitude */
    struct vectf vect_force = origin->velocity;
    float sqrSumForce = ve_sumOfSquare(vect_force);
    float mag_force = root(sqrSumForce); 

    // iterator logic start
    struct physItem *target = pList->head;
    int num = pList->size;

    // Targets with minimum dist to origin
    struct physItem* minTargs[COL_MAX];
    float minSqrSumDist=INT_MAX;
    int minTargsIndex = 0;

    // collision helper variables
    float theta_col , sine_col , dist_target_col , tangent_len_col, radius_col;
    struct vectf target_pos;

    for (int i = 0; i < num && target != NULL; i++, target = target->next){
    ////////////////////////////////////////////////////////////
        if (target == origin) continue;

        /* target: origin -> target
         * force: origin -> origin + force
         */ 
        struct vectf vect_target = ve_sub(target->coords, origin->coords);
        float sqrSumTarget = ve_sumOfSquare(vect_target);
        float radius = target->radius + origin->radius;

        // CHECK 1: magnitudeForce + radius < magnitudeTarget 
        // via squared both sides
        if( sqrSumForce + sqr(radius) + 2.f * mag_force * radius
            < sqrSumTarget ) continue;

        // second check, obtuse angle
        float dotProduct = ve_dot(vect_target, vect_force);
        float theta = ve_angle(sqrSumTarget, sqrSumForce, dotProduct);
        float sine = sinf(theta);
        float dist_target = root(sqrSumTarget);
        float tangent_len = sine * dist_target;
        if (theta >= PI_HALF) continue;

        // third check, case of shorter magnitudeForce < magnitudeTarget
        // and distance is greater than radius
        if (sqrSumForce < sqrSumTarget){
            if(ve_sumOfSquare(ve_sub(vect_target,vect_force)) >= sqr(radius))
                continue; }
        // case of longer magnitudeForce, shortest distance is longer than radius
        else if ( tangent_len >= radius) continue;
        
        // add to collision consideration
        if (sqrSumTarget < minSqrSumDist){
            minSqrSumDist = sqrSumTarget;
            minTargsIndex = 1;
            minTargs[0] = target;
            theta_col = theta; 
            sine_col = sine;
            dist_target_col = dist_target;
            tangent_len_col = tangent_len;
            radius_col = radius;
            target_pos = target->coords;
        }

        else if (sqrSumTarget == minSqrSumDist){
            minTargs[minTargsIndex++] = target;
        }
    //////////////////////////////////////////////////////////////
    }
    if (minSqrSumDist != INT_MAX){
        // backtrack
        struct vectf normal = (struct vectf) {-vect_force.y, vect_force.x};
        normal = ve_scale(normal, invSqr( sqr(normal.x) + sqr(normal.y) ) * tangent_len_col );

        float backtrack = root( sqr(radius_col) - sqr(tangent_len_col) );
        struct vectf reverse = ve_scale(vect_force, -1);
        reverse = ve_scale(reverse, invSqr( sqr(reverse.x) + sqr(reverse.y) ) * backtrack );

        origin->coords = ve_add(ve_add(target_pos, normal) , reverse);

        phys_collide(origin, minTargs, minTargsIndex);
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
