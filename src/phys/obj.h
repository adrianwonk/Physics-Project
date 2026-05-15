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
        float mass;
        struct vect F_total;
    };
/******************************************************************/
