#include "../phys_physList/phys_physList.h"
#include <pthread.h>
#include <stdlib.h>

// force to velocity conversion for this timeslice
void phys_enforce(struct physItem *target, float delta){
    float mass = target -> mass;

    /* reads F_total and resets it */
    pthread_mutex_lock(&(target->F_lock));
        struct vectf force = target -> F_total;
        target -> F_total = (struct vectf){0,0};
    pthread_mutex_unlock(&(target->F_lock));
    
    struct vectf accel = ve_scale(force, inv(mass));
    struct vectf scaled = ve_scale(accel, delta);
    target -> velocity = ve_add(target->velocity, scaled);
}

// Acknowledge delta for each timeBlock, re-apply inexpired forces.
void phys_processTimedForce( struct physItem *target, float delta ){

    struct timeBlock *prev = NULL; // prev ptr is either NULL, or previous timeBlock in iteration.
    struct timeBlock *curr = target->time_q;

    while ( curr != NULL ){
        // timeBlock list for target is not empty
        
        curr->t_remain -= delta; // deplete timeBlock's remaining time after force conversion for current timeslice.

        if (curr -> t_remain <= 0.f){ // timeBlock expired after most recent timeslice

            if (prev == NULL){ // [case] current is the front of queue
                
                target -> time_q = curr -> next; 
                free(curr);
                curr = target -> time_q; 
                continue;

            } else { // [case] current expired timeBlock is not head.

                prev->next = curr->next;
                free(curr);
                curr = prev -> next;
                continue;
            }
        }

        pi_applyForce( target, curr->force );

        prev = curr;
        curr = curr->next;
    }
}
