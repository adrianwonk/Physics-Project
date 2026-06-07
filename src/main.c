#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "main.h"

#include "phys/phys.h"
#include "math/math.h"

void getCenter(int*, int*);
void drawGrid(int startx, int starty);
void addFloor(struct physList * list);
struct ball * floorB[W];
struct physItem * floorP[W];

int main(){
    // physics related, add some balls to list
    struct physList *pl = pl_create();

    addFloor(pl);

    struct ball *b1 = constructItem(3, 'o');
    struct ball *b2 = constructItem(3, 'o');

    pl_subscribe(b1, 5,  5, pl, 1, true );
    pl_subscribe(b2, 1, 1, pl, 1, false );

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
        floorB[i] = constructItem(3, 'o');
        pl_subscribe(floorB[i],  i,  H-1, pl, 0.5f, false );
    }
}
