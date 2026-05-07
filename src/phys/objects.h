#pragma once
#include "../math/math.h"
/* physList
 *  tracks size and head ptr.
 * */
struct physList {
    int size;

    // first element of the list
    struct physItem *head;
};
/****************************************************/

/* physItem
 *  phys wrapper for frontend objects.
 *  */
struct physItem {
    void *obj; // item //
    
    // cumulative forces to be applied next iteration. positive x is right, positive y is down
    struct vect forces;
    bool processFlag;
    struct vect coords;
    struct physItem *next;
    float radius;
};
