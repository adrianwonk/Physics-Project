#pragma once
#include "../math/math.h"
/* objects */ 
//----------------------------------------------------/
    struct physList { // each physList manages memory for physItems.
        int size;

        // first element of the list
        struct physItem *head;
    };

    struct physItem { // phys wrapper for frontend objects.
        void *obj; // item //
        
        // delta displacement applied to coords each timestep, accumulated from forces
        struct vect velocity;
        bool processFlag;
        struct vect coords;
        struct physItem *next;
        float radius;
        struct vect F_total;
    };
/******************************************************************/


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
    void pi_applyForce(struct physItem *target, struct vect force);
/****************************************************/


