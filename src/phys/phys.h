#pragma once
#include "objects.h"

/* shapes */
struct ball * constructItem(float inp_mass, char c);

/* create physList to enact forces on*/
struct physList * pl_create();
struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius, bool process);

// frees physList and all physItems within.
void pl_destroy(struct physList *pList);

/****************************************************/

/* physics stuff*/
void phys_iterate(struct physList *, int, int, bool);
void pi_applyForce(struct physItem *target, struct vect force);

/****************************************************/


