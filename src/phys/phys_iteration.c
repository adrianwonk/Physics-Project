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
void phys_collide(struct physItem *target, struct physItem *victim, float angle);
/****************************************************/

/* phys iteration 
 * */
void phys_iterate(struct physList *pList, int topLeftX,
               int topLeftY, int W, int H){
    // iterator logic start
    struct physItem *target = pList->head;
    int num = pList->size;
    for (int i = 0; i < num && target != NULL; i++){
    ////////////////////////////////////////////////////////////
        if (target -> processFlag){
            phys_gravity(target);
            phys_detect(target, pList);
        }
        mvprintw(topLeftY+ target->coords.y,
                    topLeftX+ target->coords.x,
                    (char*)pi_getItem(target));

    ///////////////////////////////////////////////////////////
        target = target->next;
    }
    // iteator logic end
}

void phys_gravity(struct physItem *target){
    pi_applyForce(target, (struct vect) {0, 1});
}

void phys_detect(struct physItem *origin, struct physList * pList){
    struct vect vect_force = origin->forces;
    float sqrSumForce = ve_sumOfSquare(vect_force);
    float mag_force = root(sqrSumForce); 

    // iterator logic start
    struct physItem *target = pList->head;
    int num = pList->size;
    for (int i = 0; i < num && target != NULL; i++){
    ////////////////////////////////////////////////////////////

        if (target == origin) continue;

        // collision detection
        // checks if origin touches target
        ////////////////////////////////////////////
        
        /* target: origin -> target
         * force: origin -> origin + force
         */ 
        struct vect vect_target = ve_sub(target->coords, origin->coords);
        float sqrSumTarget = ve_sumOfSquare(vect_target);
        float radius = target->radius + origin->radius;

        // first check, magnitudeForce + radius < magnitudeTarget 
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
        
        // collide
	phys_collide(origin, target, theta);
        
    //////////////////////////////////////////////////////////////
        target = target->next;
    }
    // iterator logic end

    // no collision detected
    origin->coords = ve_add(origin->coords, origin->forces);
    origin->processFlag = false;
}

void phys_collide(struct physItem *target, struct physItem *victim, float angle){
    target->processFlag = false;
    victim->processFlag = false;
    target->coords = (struct vect){0,0};
    victim->coords = (struct vect){3,0};
}
