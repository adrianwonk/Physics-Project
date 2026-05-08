#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "phys/phys.h"
#include "math/math.h"
#define H 20 // grid Height
#define W 60 // grid Width

void getCenter(int*, int*, int, int);
void drawGrid(int height, int width, int startx, int starty);
void addFloor(struct physList * list);
struct ball * floorB[W];
struct physItem * floorP[W];

int main(){
    // physics related, add some balls to list
    struct physList *pl = pl_create();

    addFloor(pl);

    struct ball *b1 = constructItem(3, 'o');
    struct ball *b2 = constructItem(3, 'o');
    struct ball *b3 = constructItem(3, 'o');

    pl_subscribe(b1, 0,  18, pl, 1, false );
    pl_subscribe(b2, 2,  18, pl, 1, false );
    pl_subscribe(b3, 1,  0, pl, 0.5f, true );

    // init window
    initscr();
    cbreak();
    noecho();
    curs_set(0); // hides cursor

    int topLeftX, topLeftY;
    getCenter(&topLeftX, &topLeftY, W, H);

    while (1){
        erase();
        drawGrid(H, W, topLeftX, topLeftY);
        phys_iterate(pl, topLeftX, topLeftY, W, H); // updates items and draws them too.
        refresh();
        usleep(800 * 1000);
    }
    //// free allocated memory
    endwin();


    free(b1);
    free(b2);
    pl_destroy(pl);
    free(b3);

    return 0;
}

void drawGrid(int height, int width, int startx, int starty){
    int limit = height * width;
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++)
            mvprintw(starty + i, startx + j, ".");
    }
}

void getCenter(int* x, int* y, int width, int height){
    int wHeight;
    int wWidth;
    getmaxyx(stdscr, wHeight, wWidth);
    *x = wWidth / 2 - width / 2;   
    *y = wHeight / 2 - height / 2;  
}

void addFloor(struct physList * pl){
    for (int i = 0; i < W; i++){
        floorB[i] = constructItem(3, 'o');
        pl_subscribe(floorB[i],  i,  H-1, pl, 0.5f, false );
    }
}
