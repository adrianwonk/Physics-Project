#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "phys/phys.h"
#include "math/math.h"
#include "const.h"

void getCenter(int*, int*);
void drawGrid(int startx, int starty);

int main(){
    struct physList *pl = pl_create();
    int topLeftX, topLeftY;
    getCenter(&topLeftX, &topLeftY);

    char character[2] = {'e','\0'};
    struct physItem *bird = pl_subscribe(character, W/2, H/2, pl, 0.5f, true, 0.5f);
    /* ***********************************************/

    // ncurses window setup
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    curs_set(0); // hides cursor
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

        if (getch() != ERR){
            pi_applyForce(bird, (struct vect) {0,-10});
        }

        clock_gettime(CLOCK_MONOTONIC, &ts);
        t2 = ts.tv_sec;
        phys_iterate(pl, topLeftX, topLeftY, true, t2-t1);
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec;

        refresh();
        usleep(800 * 1000);
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
