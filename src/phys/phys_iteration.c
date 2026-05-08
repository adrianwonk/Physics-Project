#include <limits.h>
#include "phys_physList.h"
#include "objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#define PI 3.1415927
#define PI_HALF 1.5707964

/* forward declarations
 * */
//----------------------------------------------------/
void phys_gravity(struct physItem *target);
void phys_detect(struct physItem *origin, struct physList * pList);
void phys_collide(struct physItem *target, struct physItem **, int);
/****************************************************/

/* phys iteration 
 * */
void phys_iterate(struct physList *pList, int topLeftX,
               int topLeftY, int W, int H){
    // iterator logic start
    struct physItem *target = pList->head;
    int num = pList->size;
    for (int i = 0; i < num && target != NULL; i++, target = target->next){
    ////////////////////////////////////////////////////////////
        if (target -> processFlag){
            phys_gravity(target);
            phys_detect(target, pList); // move and collide
        }
        mvprintw(topLeftY+ target->coords.y,
                    topLeftX+ target->coords.x,
                    (char*)pi_getItem(target));

    ///////////////////////////////////////////////////////////
    }
    // iteator logic end
}

void phys_gravity(struct physItem *target){
    pi_applyForce(target, (struct vect) {2, 1});
}

void phys_detect(struct physItem *origin, struct physList * pList){
    /* 1. force applied to origin
     * 2. pre-magnitude 
     * 3.  magnitude */
    struct vect vect_force = origin->forces;
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
        struct vect vect_target = ve_sub(target->coords, origin->coords);
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
    //TODO finish integrating pi_list change to minTargsList
    if (minSqrSumDist != INT_MAX){
        phys_collide(origin, minTargs, minTargsIndex);
        return;    
    } else {
        origin->coords = ve_add(origin->coords, origin->forces);
    }
}

void phys_collide(struct physItem *target, struct physItem **victimArr, int n){
    target->processFlag = false;
    target->coords = (struct vect){target->coords.x,22};
    for (int i = 0; i < n; i++){
        victimArr[i]->processFlag = false;
        victimArr[i]->coords = (struct vect){target->coords.x,23 + i};
    }
}
