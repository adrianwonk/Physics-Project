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

int main(){
    // physics related, add some balls to list
    struct physList *pl = pl_create();
    int topLeftX, topLeftY;
    getCenter(&topLeftX, &topLeftY);
    /* ***********************************************/

    // ncurses window setup
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    curs_set(0); // hides cursor
    /* ***********************************************/

    while (1){

        erase();
        drawGrid(topLeftX, topLeftY);
        phys_iterate(pl, topLeftX, topLeftY, false); // updates items and draws them too.

        refresh();
        usleep(800 * 1000);
    }
    
    // destructors
    endwin();
    free(b1);
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
