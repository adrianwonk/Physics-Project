#pragma once
#include "../math/math.h"
#include "obj.h"

/* physList functions */
//------------------------------------------------------------------/
    struct physList * pl_create();
struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius, bool process, float mass);

    // frees physList and all physItems within.
    void              pl_destroy(struct physList *pList);
/******************************************************************/

/* physics stuff*/
    void phys_iterate(struct physList *, int, int, bool, float);
    void pi_applyForce(struct physItem *, struct vect);
/****************************************************/


