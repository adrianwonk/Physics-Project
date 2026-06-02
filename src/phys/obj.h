#pragma once
#include <stdbool.h>
#include "../math/math.h"
#include <pthread.h>

struct timeBlock {
    float t_remain;
    struct vectf force;
    struct timeBlock *next;
};

/* objects */ 
//----------------------------------------------------/
    struct physList { // each physList manages memory for physItems.
        int size;

        // first element of the list
        struct physItem *head;
    };

    struct physItem { // phys wrapper for frontend objects.
        char *obj; // item //
        
        // delta displacement applied to coords each timestep, accumulated from forces
        struct vectf velocity;
        bool processFlag;
        struct vectf coords;
        struct physItem *next;
        float radius;
        float mass;
        struct vectf F_total;
        pthread_mutex_t F_lock;
        struct timeBlock *time_q;
        struct vectf gravity;
    };
/******************************************************************/
