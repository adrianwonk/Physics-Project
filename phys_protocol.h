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
struct vect ve_add(struct vect v1, struct vect v2);
struct vect ve_sub(struct vect v1, struct vect v2);
float ve_sumOfSquare(struct vect v);
float ve_dot(struct vect v1, struct vect v2);
float ve_angle(float sqr1, float sqr2, float dprod);
/****************************************************/

/* physList
 *  tracks size and head ptr.
 * */
struct physList {
    int size;

    // first element of the list
    struct physItem *head;
};
struct physList * pl_create();
void pl_destroy(struct physList *);
void pl_subscribe(struct physItem *, struct physList *);
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

struct physItem *
     pi_create(void *obj, int startx, int starty);

bool pi_isOperational(struct physItem *target);
void pi_applyForce(struct physItem *target, struct vect force);
/****************************************************/

/* functions phys iteration
 * */
void phys_iterate(struct physList *, int, int, int, int);
void phys_collide(struct physItem *target, struct physItem *victim, float angle);
void phys_process(struct physItem *target);
/****************************************************/

/* math */
float invSqr( float number );
float inv( float number );
float sqr( float number );
float root( float number );
