#pragma once
/* Data structures:
 *   1. vect
 *   2. physList
 *   3. physItem
 * */

struct vect;
struct physList;
struct physItem;

/* vect
 * */
struct vect {
    int x; int y;
};
/****************************************************/

/* physList
 *  tracks size and head ptr.
 * */
struct physList {
    int size;

    // first element of the list
    struct physItem *head;
};
/****************************************************/

/* physItem
 *  phys wrapper for frontend objects.
 *  */
struct physItem {
    void *obj; // item //
    
    // cumulative forces to be applied next iteration. positive x is right, positive y is down
    struct vect forces;
    bool processFlag;
    struct vect coords;
    struct physItem *next;
    float radius;
};
/****************************************************/


