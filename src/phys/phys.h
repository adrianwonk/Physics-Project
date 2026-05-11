#pragma once
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


/* physList functions */
//------------------------------------------------------------------/
    struct physList * pl_create();
struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius, bool process, float mass);

    // frees physList and all physItems within.
    void              pl_destroy(struct physList *pList);
/******************************************************************/

/* physics stuff*/
    void phys_iterate(struct physList *, int, int, bool);
    void pi_applyForce(struct physItem *target, struct vect force);
/****************************************************/


