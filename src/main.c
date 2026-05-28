#include <locale.h>
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

// void *input_listener(void * args){
    
// }
// pthread_create();
int main(){
    setlocale(LC_ALL, "C.UTF-8");

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
        t1 = ts.tv_sec + ts.tv_nsec * 1e-9f;
    /* ***********************************************/

    while (1){
        erase();
        drawGrid(topLeftX, topLeftY);

        // jump
        if (getch() != ERR){
            if (pi_isFalling(bird)) pi_resetVelocity(bird);
            pi_timedForce(bird, 1.f, (struct vectf) {0,-4});
            // log
            log_item("PRESSED!: " , bird);
        }

        clock_gettime(CLOCK_MONOTONIC, &ts);
        t2 = ts.tv_sec + ts.tv_nsec * 1e-9f;
        phys_iterate(pl, topLeftX, topLeftY, true, t2-t1);
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec + ts.tv_nsec * 1e-9f;

        refresh();
        napms(16);
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
