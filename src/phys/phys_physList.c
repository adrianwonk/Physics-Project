#include "phys.h"
#include <stdlib.h>
/* physList and physItem functions
 *  each physList manages memory for physItems.

/* physList
 */

struct physItem *pi_create(void *, int , int , float , float );
void pi_destroy(struct physItem *);
void pl_addItem(struct physItem *, struct physList *);


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

struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius, bool process, float mass){
    struct physItem *tmp = pi_create(obj,startx,starty,radius,mass);
    tmp -> processFlag = process;
    pl_addItem(tmp,list);
    return tmp;
}

    void pl_addItem(struct physItem *pItem, struct physList *pList){
        pItem->next = pList->head;
        pList->head = pItem;
        pList->size++;
    }
/****************************************************/

/* physItem
 * */
struct physItem *pi_create(void *obj, int startx, int starty, float radius, float mass) {
    struct physItem *target = malloc(sizeof(struct physItem));
    *target = (struct physItem) {
        .obj = obj,
        .velocity = (struct vect){0,0},
        .processFlag = false,
        .coords = (struct vect){startx, starty},
        .next = NULL,
        .radius = radius,
        .mass = mass,
        .F_total  = (struct vect) {0,0}
    };
    return target;
}

void * pi_getItem(struct physItem * ptr){
    return ptr -> obj;
}

void pi_destroy(struct physItem *target){
    free(target);
}

bool pi_isOperational(struct physItem *target){
    return target->processFlag;
}

void pi_applyForce(struct physItem *target, struct vect force){
    target -> F_total = ve_add(target -> F_total, force);
    
    target -> processFlag = true;
}
    /* float mass = target -> mass; */
    /* struct vect accel = ve_scale(force, inv(mass)); */
    /* target -> deltaD = ve_add(target->deltaD, force); */
