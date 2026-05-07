#pragma once
#include "objects.h"

/* shapes */
struct ball * constructItem(float inp_mass, char c);

/* PhysList stuff*/
struct physList * pl_create();

// frees physList and all physItems within.
void pl_destroy(struct physList *pList);

void pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius);
/****************************************************/

/* physics stuff*/
void phys_iterate(struct physList *, int, int, int, int);
/****************************************************/


