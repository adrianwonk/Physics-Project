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
