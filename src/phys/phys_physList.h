#pragma once
#include "phys.h"
/* physList and physItem functions
 *  each physList manages memory for physItems.
/****************************************************/

/* physList
 * */
//----------------------------------------------------/
struct physList * pl_create();

// frees physList and all physItems within.
void pl_destroy(struct physList *pList);

void pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius);

void pl_addItem(struct physItem *pItem, struct physList *pList);
/****************************************************/

/* physItem forward declaration. Hide from outside.
 * */
//----------------------------------------------------/
struct physItem *pi_create(void *obj, int startx, int starty, float radius); 
void *pi_getItem(struct physItem *target);
void pi_destroy(struct physItem *target);

bool pi_isOperational(struct physItem *target);

void pi_applyForce(struct physItem *target, struct vect force);
/****************************************************/
