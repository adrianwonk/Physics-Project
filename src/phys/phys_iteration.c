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

/* forward declarations
 * */
//----------------------------------------------------/
void phys_gravity(struct physItem *, float, float);
void phys_detect(struct physItem *, struct physList *, float);
void phys_collide(struct physItem *, struct physItem **, int);
void phys_enforce(struct physItem *, float);
/****************************************************/

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
/* phys iteration 
 * */
void phys_iterate(struct physList *pList, int topLeftX,
               int topLeftY, bool clip, float delta){
    // iterator logic start
    struct physItem *target = pList->head;
    int num = pList->size;
    for (int i = 0; i < num && target != NULL; i++, target = target->next){
    ////////////////////////////////////////////////////////////
        if (target -> processFlag){
            phys_gravity(target, 1, delta);
            
            // 1. Process force, accumulate velocity
            phys_enforce(target, delta);
            // n. Process displacement change
            phys_detect(target, pList, delta); // move and collide
        }
        if ( !clip || (target->coords.x < W && target->coords.x >= 0 && target -> coords.y < H && target -> coords.y >= 0) ){
            log_item("", target);

            struct vectd coords = ve_squash(target->coords);
            mvprintw(topLeftY+ coords.y,
                topLeftX+ coords.x,
                (char*)pi_getItem(target));

    ///////////////////////////////////////////////////////////
        }
    // iteator logic end
    }
}

void phys_gravity(struct physItem *target, float gravity, float delta){
    // bypasses force system cuz all objects have the same acceleration!
    target -> velocity = ve_add(target -> velocity,
            (struct vectf){0, gravity * delta}
    );
}

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
    struct physItem* minTargs[5];
    float minSqrSumDist=INT_MAX;
    int minTargsIndex = 0;

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
        if (theta >= PI_HALF) continue;

        // third check, case of shorter magnitudeForce < magnitudeTarget
        // and distance is greater than radius
        if (sqrSumForce < sqrSumTarget){
            if(ve_sumOfSquare(ve_sub(vect_target,vect_force)) >= sqr(radius))
                continue;
        } else {
        // case of longer magnitudeForce, shortest distance is longer than radius
            if (sqr(sinf(theta)) * sqrSumTarget >= sqr(radius)) continue;
        }
        
        // add to collision consideration
        if (sqrSumTarget < minSqrSumDist){
            minSqrSumDist = sqrSumTarget;
            minTargsIndex = 1;
            minTargs[0] = target;
        }

        else if (sqrSumTarget == minSqrSumDist){
            minTargs[minTargsIndex++] = target;
        }
    //////////////////////////////////////////////////////////////
    }
    if (minSqrSumDist != INT_MAX){
        phys_collide(origin, minTargs, minTargsIndex);
        return;    
    } else {
        origin->coords = ve_add(origin->coords, ve_scale(origin->velocity, delta));
    }
}

void phys_collide(struct physItem *target, struct physItem **victimArr, int n){
    target->processFlag = false;
    target->coords = (struct vectf){target->coords.x,22};
    for (int i = 0; i < n; i++){
        victimArr[i]->processFlag = false;
        victimArr[i]->coords = (struct vectf){target->coords.x,23 + i};
    }
}
