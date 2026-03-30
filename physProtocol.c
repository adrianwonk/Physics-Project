#include "physProtocol.h"
#include <stdio.h>
#include <stdlib.h>

void createPhysItem(void *obj, int startx, int starty, struct physItem *target){
    *target = (struct physItem)
        {
            obj,
            (struct vect){0,0},
            false,
            (struct vect){startx, starty}
        };
}

void destroyPhysItem(struct physItem *target){
    free(target);
}

struct physItem *getPhysItem(struct plElem *target){
    return target->curr;
}

void *getItem(struct physItem * target){
    return target->i;
}

bool skipProcessing(struct physItem *target){
    return !target->processFlag;
}

void processPhysItem(struct physItem *target){
    return;
}

void plSubscribe(struct physItem *pItem, struct physList *pList){
    // allocate a new plElem for physItem
    struct plElem *ptr = (struct plElem *) malloc(sizeof(struct plElem));
    struct plElem data = {
        pItem, pList->head
    };
    *ptr = data;
    pList->head = ptr;
    pList->size++;
}

void plInit(struct physList *pList){
    pList->size = 0;
    pList->head = NULL;
}

void plFree(struct physList *pList){
    // how many plElem containers to free
    int num = pList->size;

    struct plElem *tmp = pList->head;
    free(pList);

    for (int i = 0; i < num && tmp != NULL; i++){
        struct plElem *del = tmp;
        tmp = tmp->next;
        free(del);
    }
}

void progressPlElem(struct plElem **target){
    struct plElem *ptr = *target;
    ptr = ptr->next;
}

void plIterate(struct physList *pList){
    struct plElem *tmp = pList->head;
    int num = pList->size;

    for (int i = 0; i < num && tmp != NULL; i++){
        // do something with tmp.
        struct physItem *pi = getPhysItem(tmp);
        if (!skipProcessing(pi)){
            processPhysItem(pi);
        }
        progressPlElem(&tmp);
    }
}
