#pragma once
#include <stdbool.h>
#include "obj.h"

struct physList * pl_create();

// frees physList and all physItems within.
void pl_destroy(struct physList *pList);
struct physItem * pl_subscribe(void *obj, int startx, int starty, struct physList *list,
                  float radius, bool process, float mass);

/* physItem
 * */
void * pi_getItem(struct physItem *);
bool pi_isOperational(struct physItem *);
void pi_applyForce(struct physItem *, struct vectf);
void pi_resetVelocity(struct physItem *target);
bool pi_isFalling(struct physItem * ptr);
