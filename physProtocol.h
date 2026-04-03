/* physProtocol.h
 * 
 * Layer 2
 * physList: stores physItems which are consumed by the physics engine every iteration.
 * 
 * Layer 1
 * plElem: services physList by connecting physItems together
 * physItem: Encapsulates object with forces to be applied to it.
*/

// stores the cumulative forces to be applied to the designated object.
struct vect {
    int x; int y;
};

struct physItem {
    void *i; // item //
    
    // cumulative forces to be applied next iteration. positive x is right, positive y is down
    struct vect forces;
    bool processFlag;
    struct vect coords;
};
void createPhysItem(void *obj, int startx, int starty, struct physItem *target);
void processPhysItem(struct physItem *target);
void *getItemFromPhysItem(struct physItem * target);
bool skipProcessing(struct physItem *target);
void destroyPhysItem(struct physItem *);

struct plElem {
    struct physItem *curr;
    struct plElem *next;
};
struct physItem *getPhysItemFromPlElem(struct plElem *);

struct physList {
    int size;

    // first element of the list
    struct plElem *head;
};
void progressPlElem(struct plElem **target);
void plInit(struct physList *);
void plFree(struct physList *);
void plSubscribe(struct physItem *, struct physList *);
void plIterate(struct physList *, int topLeftX, int topRightY);
