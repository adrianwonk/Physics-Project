#pragma once
#include "phys.h"
/* objects */ 
//----------------------------------------------------/
    struct physList { // each physList manages memory for physItems.
        int size;

        // first element of the list
        struct physItem *head;
    };

    struct physItem { // phys wrapper for frontend objects.
        void *obj; // item //
        
        // cumulative forces to be applied next iteration. positive x is right, positive y is down
        struct vect forces;
        bool processFlag;
        struct vect coords;
        struct physItem *next;
        float radius;
    };
/******************************************************************/

/* physList and physItem functions, mirrors phys_physList.c implmentations */
//----------------------------------------------------/
    /* physList functions */
    //----------------------------------------------------/
        struct physList * pl_create();

        // frees physList and all physItems within.
        void pl_destroy(struct physList *pList);

        struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                          float radius, bool process);

        void pl_addItem(struct physItem *pItem, struct physList *pList);
    /****************************************************/

    /* physItem functions */
    //----------------------------------------------------/
        struct physItem *pi_create(void *obj, int startx, int starty, float radius); 
        void *pi_getItem(struct physItem *target);
        void pi_destroy(struct physItem *target);

        bool pi_isOperational(struct physItem *target);

        void pi_applyForce(struct physItem *target, struct vect force);
    /****************************************************/
/****************************************************/
