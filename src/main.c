#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#include "log.h"
#include "phys/phys.h"
#include "math/math.h"
#include "const.h"

void getCenter(int*, int*);
void drawGrid(int startx, int starty);

/* ASYNC ADD FORCE*****/
struct thread_args = {
    float duration;
    struct vectf amt;
    struct physItem *target;
};

void addForceOverTime(struct physItem * target, float sec){
    // warning! probably better to store references in main.c so we can kill threads running too long.
    pthread_t thread;
    pthread_create(&thread, NULL, thread_helper, NULL);
    // warning! probably better not to detach thread.
    pthread_detach(thread);
}

void *thread_helper(void *input) {
    // semaphore to signal 
    struct thread_args *args = (struct thread_args *)input;
    float acc;
    sem_t signal;
    sem_init(&sem, 0, 0);

    while (1){
        pi_applyForce( args -> target,  args -> amt );
        //
        // wait for phys_iterate to increment acc
        sem_wait(&sem);
        if ( acc > args -> duration ) return NULL;
    }
}
/*****/

int main(){
    struct physList *pl = pl_create();
    int topLeftX, topLeftY;

    char character[2] = {'e','\0'};
    struct physItem *bird = pl_subscribe(character, W/2, H/2, pl, 0.5f, true, 1.f);
    /* ***********************************************/

    // ncurses window setup
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    curs_set(0); // hides cursor
    getCenter(&topLeftX, &topLeftY);
    /* ***********************************************/
    
    // timer setup
    struct timespec ts;
    float t1;
    float t2;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    t1 = ts.tv_sec;
    /* ***********************************************/

    while (1){

        erase();
        drawGrid(topLeftX, topLeftY);

        // jump
        if (getch() != ERR){
            if (pi_isFalling(bird)) pi_resetVelocity(bird);
            pi_applyForce(bird, (struct vectf) {0,-5});
            // log
            log_item("PRESSED!: " , bird);
        }


        clock_gettime(CLOCK_MONOTONIC, &ts);
        t2 = ts.tv_sec + ts.tv_nsec * 1e-9f;
        phys_iterate(pl, topLeftX, topLeftY, true, t2-t1);
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec + ts.tv_nsec * 1e-9f;

        refresh();
        /* usleep(800 * 1000); */
    }
    
    // destructors
    endwin();
    pl_destroy(pl);
    /* ***********************************************/

    return 0;
}

void drawGrid(int startx, int starty){
    int limit = H * W;
    for (int i=0; i < H; i++){
        for (int j=0; j < W; j++)
            mvprintw(starty + i, startx + j, ".");
    }
}

void getCenter(int* x, int* y){
    int wHeight;
    int wWidth;
    getmaxyx(stdscr, wHeight, wWidth);
    *x = wWidth / 2 - W / 2;   
    *y = wHeight / 2 - H / 2;  
}
