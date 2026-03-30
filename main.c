#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// functions to manage grid buffer
#include "grid.c"

// describes a ball structure
#include "ball.h"

#include "physProtocol.h"

#define H 30 // grid Height
#define W 60 // grid Width

// global variables
char *gridMatrix[H];

void getCenter(int*, int*, int, int);

int main(){
    char *const gridData = malloc(sizeof(char) * H * W);
    initGrid(gridMatrix, gridData, H, W);

    // physics related, add some balls to list
    struct physList *pl = malloc(sizeof(struct physList));
    plInit(pl);
    struct ball b = {
        10
    };

    struct ball b2 = {
        90
    };

    struct ball b3 = {
        24
    };

    struct physItem *pball = malloc(sizeof(struct physItem));
    struct physItem *pball2 = malloc(sizeof(struct physItem));
    struct physItem *pball3 = malloc(sizeof(struct physItem));
    createPhysItem(&b, W/2, H/2, pball);
    createPhysItem(&b2, W/2-10, H/2+10, pball2);
    createPhysItem(&b3, W/2+10, H/2-10, pball3);
    plSubscribe(pball, pl);
    plSubscribe(pball2, pl);
    plSubscribe(pball3, pl);

    // init window
    initscr();
    cbreak();
    noecho();
    curs_set(0); // hides cursor

    int topLeftX, topLeftY;
    getCenter(&topLeftX, &topLeftY, H, W);

    while (1){
        erase();
        drawGrid(gridMatrix, H, W, topLeftX, topLeftY);
        refresh();
    }
    //// free allocated memory
    endwin();

    free(pball);
    free(pball2);
    free(pball3);

    plFree(pl);
    destroyGrid(gridData);

    return 0;
}

void getCenter(int* x, int* y, int height, int width){
    int wHeight;
    int wWidth;
    getmaxyx(stdscr, wHeight, wWidth);
    *x = wWidth / 2 - width / 2;   
    *y = wHeight / 2 - height / 2;  
}
