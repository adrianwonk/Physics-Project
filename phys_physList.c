#include "phys_protocol.h"
/* physList and physItem functions
 *  each physList manages memory for physItems.

/* physList
 */
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
        free(del);
    }
}

void pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius){
    struct physItem *tmp = pi_create(obj,startx,starty,radius);
    pl_addItem(tmp,list);
}

void pl_addItem(struct physItem *pItem, struct physList *pList){
    pItem->next = pList->head;
    pList->head = pItem;
    pList->size++;
}
/****************************************************/

/* physItem
 * */
struct physItem *pi_create(void *obj, int startx, int starty, float radius) {
    struct physItem *target = malloc(sizeof(physItem));
    *target = (struct physItem) {
        .obj = obj,
        .forces = (struct vect){0,0},
        .processFlag = false,
        .coords = (struct vect){startx, starty},
        .next = NULL,
        .radius = radius
    };
    return target;
}

void pi_destroy(struct physItem *target){
    free(target);
}

bool pi_isOperational(struct physItem *target){
    return target->processFlag;
}

void pi_applyForce(struct physItem *target, struct vect force){
    target -> processFlag = true;
    target -> forces = veAdd(target->forces, force);
}
