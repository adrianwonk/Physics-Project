#include <criterion/criterion.h>
#include "phys.h"
#include "math.h"
#include "phys_iteration_func.h"
#include "phys_physList_func.h"
#include "ball.h"
#include <stdlib.h>
#include <stdio.h>

struct ball *b1;
struct ball *b2;
struct ball *b3;

struct physItem *p1;
struct physItem *p2;
struct physItem *p3;

struct physList *list;
void prepBalls(){
    b1 = malloc(sizeof(struct ball)); 
    b2 = malloc(sizeof(struct ball)); 
    b3 = malloc(sizeof(struct ball)); 
    constructBall(b1,10);
    constructBall(b2,90);
    constructBall(b3,24);
}

void prepPhysItem(){
    p1 = malloc(sizeof(struct physItem)); 
    p2 = malloc(sizeof(struct physItem)); 
    p3 = malloc(sizeof(struct physItem)); 
    createPhysItem(b1, 2, 4, p1);
    createPhysItem(b2, 5, 100, p2);
    createPhysItem(b3, 30, 10, p3);
    p3->processFlag = true;
}

void prepList(){
    prepBalls();
    prepPhysItem();
    list = malloc(sizeof(struct physList));
    pl_create(list);
    plSubscribe(p1,list);
    plSubscribe(p2,list);
    plSubscribe(p3,list);
}

Test(ball, constructBall){
    prepBalls();
    cr_assert(((char *) b1)[1] == '\0',"didn't recieve terminating string after first character, instead got %c", ((char *) b1)[1]);
    cr_assert(((char *) b2)[1] == '\0',"didn't recieve terminating string after first character, instead got %c", ((char *) b2)[1]);
    cr_assert(((char *) b3)[1] == '\0',"didn't recieve terminating string after first character, instead got %c", ((char *) b3)[1]);
    cr_assert(b1->mass == 10);
    cr_assert(b2->mass == 90);
    cr_assert(b3->mass == 24);
}

bool cmp_vects(struct vect v1, struct vect v2){
    return v1.x == v2.x && v1.y == v2.y;
}

Test(physprotocol, createPhysItem){
// void createPhysItem(void *obj, 
    // int startx, int starty, struct physItem *target);
    
    prepBalls();
    prepPhysItem();
    // testing item pointers
    cr_assert(p1->i == b1, "item pointer not correct for p1 b1");
    cr_assert(p2->i == b2, "item pointer not correct for p2 b2");
    cr_assert(p3->i == b3, "item pointer not correct for p3 b3");

    // testing forces
    cr_assert(cmp_vects(p1->forces,(struct vect){0,0}), "forces not correct for p1");
    cr_assert(cmp_vects(p2->forces,(struct vect){0,0}), "forces not correct for p2");
    cr_assert(cmp_vects(p3->forces,(struct vect){0,0}), "forces not correct for p3");
    
    // testing processFlag
    cr_assert(p1->processFlag == false);
    cr_assert(p2->processFlag == false);
    cr_assert(p3->processFlag == false);

    // testing coords;
    cr_assert(cmp_vects(p1->coords,(struct vect){2,4}));
    cr_assert(cmp_vects(p2->coords,(struct vect){5,100}));
    cr_assert(cmp_vects(p3->coords,(struct vect){30,10}));
}

Test(physProtocol, plSubscribe){
    prepBalls();
    prepPhysItem();
    struct physList *list = malloc(sizeof(struct physList));
    pl_create(list);
    plSubscribe(p1,list);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head)) == b1);
    plSubscribe(p2,list);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head)) == b2);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head->next)) == b1);
    plSubscribe(p3,list);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head)) == b3);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head->next)) == b2);
    cr_assert(getItemFromPhysItem(getPhysItemFromPlElem(list->head->next->next)) == b1);
}

Test(physProtocol, progressPlElem){
    prepList();
    struct plElem *tmp = list->head;

    // cmps ptr to item via current physItem, then increments tmp
    cr_assert(tmp->curr->i == b3);
    progressPlElem(&tmp);
    cr_assert(tmp->curr->i == b2);
    progressPlElem(&tmp);
    cr_assert(tmp->curr->i == b1);
    progressPlElem(&tmp);
    cr_assert(tmp == NULL);
}

