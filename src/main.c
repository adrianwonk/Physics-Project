#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "phys/phys.h"
#include "math/math.h"
#include "const.h"

void getCenter(int*, int*);
void drawGrid(int startx, int starty);
void addFloor(struct physList * list);

struct physItem * floorP[W];

int main(){
    // physics related, add some balls to list
    struct physList *pl = pl_create();

    addFloor(pl);

    struct physitem *b1 = pl_subscribe("o", 5, 3, pl, 0.5, true, 1, (struct vectf){0,-1});
    struct physitem *b2 = pl_subscribe("f", 10, 7, pl, 0.5, true, 1, (struct vectf){0,-1});

    // init window
    initscr();
    cbreak();
    noecho();
    curs_set(0); // hides cursor

    int topLeftX, topLeftY;
    getCenter(&topLeftX, &topLeftY);

    while (1){
        erase();
        drawGrid(topLeftX, topLeftY);

        phys_iterate(pl, topLeftX, topLeftY, false); // updates items and draws them too.
        refresh();
        usleep(800 * 1000);
    }
    //// free allocated memory
    endwin();


    free(b1);
    free(b2);
    pl_destroy(pl);

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

void addFloor(struct physList * pl){
    for (int i = 0; i < W; i++){
        pl_subscribe("-", 5, 3, pl, 0.5, false, 1, (struct vectf){0,0});
    }
}
