#include "../obj.h"
#include <stdbool.h>
#include <stdlib.h>
/* physList and physItem functions
 *  each physList manages memory for physItems.*/


// forward declarations
struct physItem *pi_create(char *, int , int , float , float, struct vectf );
void pi_destroy(struct physItem *);
void pl_addItem(struct physItem *, struct physList *);
void pi_applyForce(struct physItem *, struct vectf );
/*********************************************************/

void pi_timedForce(struct physItem *target, float sec, struct vectf force ){
    // requeue logic
    // MEM CREATE ============================
    struct timeBlock *tmp = malloc(sizeof(struct timeBlock));
    // =========================================

    *tmp = (struct timeBlock){
        .t_remain = sec,
        .force = force,
        .next = target->time_q
    };

    target->time_q = tmp;

    // first force application
    pi_applyForce ( target, force );
}

struct physList * pl_create(){
    struct physList *tmp = malloc(sizeof(struct physList));
    *tmp = (struct physList) {
        .size = 0,
        .head = NULL
    };
    return tmp;
}

// frees physList and all physItems within.
void pl_destroy(struct physList *pList){
    int num = pList->size;

    struct physItem *tmp = pList->head;
    free(pList);

    for (int i = 0; i < num && tmp != NULL; i++){
        struct physItem *del = tmp;
        tmp = tmp->next;
        pi_destroy(del);
    }
}

struct physItem * pl_subscribe(char *obj, int startx, int starty, struct physList *list,
                  float radius, bool process, float mass, struct vectf gravity){
    struct physItem *tmp = pi_create(obj,startx,starty,radius,mass,gravity);
    tmp -> processFlag = process;
    pl_addItem(tmp,list);
    return tmp;
}

//===HIDE===================================================
    void pl_addItem(struct physItem *pItem, struct physList *pList){
        pItem->next = pList->head;
        pList->head = pItem;
        pList->size++;
    }
//==========================================================
/****************************************************/

/* physItem */
//===HIDE===================================================
    /* pis are physList layer primitives only. */
    struct physItem *pi_create(char *obj, int startx, int starty, float radius, float mass, struct vectf gravity) {
        struct physItem *target = malloc(sizeof(struct physItem));
        *target = (struct physItem) {
            .obj = obj,
            .velocity = (struct vectf){0,0},
            .processFlag = false,
            .coords = (struct vectf){startx, starty},
            .next = NULL,
            .radius = radius,
            .mass = mass,
            .F_total  = (struct vectf) {0,0},
            .gravity = gravity
        };
        pthread_mutex_init(&(target->F_lock), NULL);
        return target;
    }

    /* we do not want users to manage pi memory */
    void pi_destroy(struct physItem *target){
        pthread_mutex_destroy(&(target->F_lock));
        free(target);
    }
//==========================================================

void * pi_getItem(struct physItem * ptr){
    return ptr -> obj;
}

bool pi_isFalling(struct physItem * ptr){
    return ptr -> velocity.y > 0;
}


bool pi_isOperational(struct physItem *target){
    return target->processFlag;
}

void pi_applyForce(struct physItem *target, struct vectf force){
    pthread_mutex_lock(&(target->F_lock));
    target -> F_total = ve_add(target -> F_total, force);
    target -> processFlag = true;
    pthread_mutex_unlock(&(target->F_lock));
}

void pi_resetVelocity(struct physItem *target){
    target -> velocity = (struct vectf) {0, 0};
}
