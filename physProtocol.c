#include "physProtocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

/* 1. Allocate memory for physList, and pass into plInit for default values.
* 2. create physItem elements separately, and subscribe them to the list.
* 3. Change the force vectors of each physItem separately. */

void plSubscribe(struct physItem *pItem, struct physList *pList){
    // allocate a new plElem for physItem
    struct plElem *ptr = malloc(sizeof(struct plElem));
    *ptr = (struct plElem){
        pItem, pList->head
    };
    pList->head = ptr;
    pList->size++;
}
void applyGravity(struct physItem *target){
    piApplyForce(pitem, (struct vect) {0, 1});
}

void plIterate(struct physList *pList, int topLeftX,
               int topLeftY, int W, int H){
    struct plElem *tmp = pList->head;
    int num = pList->size;

    for (int i = 0; i < num && tmp != NULL; i++){
        struct physItem *pitem = getPhysItemFromPlElem(tmp);
        if ()
        applyGravity(pitem);
        if (pitem -> processFlag){
            scanSurroundings(pitem);
        }
        mvprintw(topLeftY+ pitem->coords.y,
                    topLeftX+ pitem->coords.x,
                    (char*)getItemFromPhysItem(pitem));
        progressPlElem(&tmp);
    }
}

void scanSurroundings(struct physItem *target, struct physList * pList){
    // scan to see if anything is in the way.

    struct plElem *tmp = pList->head;
    int num = pList->size;
    for (int i = 0; i < num && tmp != NULL; i++){
        struct physItem *pitem = getPhysItemFromPlElem(tmp);
        if (pitem == target) continue;

        ////////////////////////////////////
        // collision detection
        // checks if target touches pitem
        ////////////////////////////////////////////
        struct vect toItem = veSub(pitem->coords, target->coords);
        float sqrForce = veSumOfSquare(target->forces);
        float sqrToItem = veSumOfSquare(toItem);
        float dotProduct = veDot(toItem, target->forces);
        //
        // approx. for longer force magnitude
        if (sqrForce > sqrToItem){
            // used for collision detection as well as feeding into collision logic
            float theta = veAngle(sqrForce, sqrToItem, dotProduct);
            
            // v is the length of the normal from forces to item
            // TODO: sinf and sqrt are from math.h lib
            float v = sinf(theta) * sqrtf(sqrToItem);
            float rec_h = invSqr(sqrToItem);
            // currently each all is 0.5 radius.
            if (sinf(theta) < 1 * rec_h){
                // collision
                collide(target, pitem, theta);
                return;
            }
        }
        progressPlElem(&tmp);
    }
    target->coords = veAdd(target->coords, target->forces);
    target->processFlag = false;
}

void collide(struct physItem *target, struct physItem *victim, float angle){
    target->processFlag = false;
    victim->processFlag = false;
    target->coords = (struct vect){0,0};
    victim->coords = (struct vect){3,0};
}

void piApplyForce(struct physItem *target, struct vect force){
    target -> processFlag = true;
    target -> forces = veAdd(target->forces, force);
}

struct vect veAdd(struct vect v1, struct vect v2){
    return (struct vect) {v1.x + v2.x, v1.y + v2.y};
}

struct vect veSub(struct vect v1, struct vect v2){
    return (struct vect) {v1.x - v2.x, v1.y - v2.y};
}

float veSumOfSquare(struct vect v){
    return v.x * v.x + v.y * v.y;
}

float veDot(struct vect v1, struct vect v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float invSqr( float number ) {
	int i;
	float halved, res;
	const float threehalfs = 1.5F;

	halved = number * 0.5F;
	res  = number;
	i  = * ( int * ) &res;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	res  = * ( float * ) &i;
	res  = y * ( threehalfs - ( halved * y * y ) );   // 1st iteration
//	res  = y * ( threehalfs - ( halved * y * y ) );   // 2nd iteration, this can be removed

	return res;
}

float veAngle(float sqr1, float sqr2, float dprod){
    float multSqr = sqr1 * sqr2;
    acos(dprod * invSqr(multSqr));
}

void createPhysItem(void *obj, int startx, int starty, struct physItem *target){
    *target = (struct physItem)
        {
            .i = obj,
            .forces = (struct vect){0,0},
            .processFlag = false,
            .coords = (struct vect){startx, starty}
        };
}

void destroyPhysItem(struct physItem *target){
    free(target);
}

void plInit(struct physList *pList){
    pList->size = 0;
    pList->head = NULL;
}

void plFree(struct physList *pList){
    // how many plElem containers to free
    int num = pList->size;

    struct plElem *tmp = pList->head;
    free(pList);

    for (int i = 0; i < num && tmp != NULL; i++){
        struct plElem *del = tmp;
        tmp = tmp->next;
        free(del);
    }
}

// getters
void progressPlElem(struct plElem **target){
    (*target) = (*target)->next;
}

struct physItem *getPhysItemFromPlElem(struct plElem *target){
    return target->curr;
}

void *getItemFromPhysItem(struct physItem * target){
    return target->i;
}

bool skipProcessing(struct physItem *target){
    return !target->processFlag;
}
