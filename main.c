#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// describes a ball structure
#include "ball.h"

#include "physProtocol.h"

#define H 20 // grid Height
#define W 60 // grid Width

void getCenter(int*, int*, int, int);
void drawGrid(int height, int width, int startx, int starty);
void addFloor(struct physList * list);
struct ball* floorB[W];
struct physItem * floorP[W];

int main(){
    // physics related, add some balls to list
    struct physList *pl = malloc(sizeof(struct physList));
    plInit(pl);

    addFloor(pl);

    struct ball *b = malloc(sizeof(struct ball));
    struct ball *b2 = malloc(sizeof(struct ball));
    struct ball *b3 = malloc(sizeof(struct ball));
    constructBall(b,10);
    constructBall(b2,90);
    constructBall(b3,24);

    struct physItem *pball = malloc(sizeof(struct physItem));
    struct physItem *pball2 = malloc(sizeof(struct physItem));
    struct physItem *pball3 = malloc(sizeof(struct physItem));
    createPhysItem(b, W/2, H/2, pball);
    createPhysItem(b2, W/2-5, H/2+5, pball2);
    createPhysItem(b3, W/2+5, H/2-5, pball3);
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
        drawGrid(H, W, topLeftX, topLeftY);
        plIterate(pl, topLeftX, topLeftY, W, H); // updates items and draws them too.
        refresh();
        sleep(1);
    }
    //// free allocated memory
    endwin();

    free(pball);
    free(pball2);
    free(pball3);

    free(b);
    free(b2);
    free(b3);

    plFree(pl);

    return 0;
}

void drawGrid(int height, int width, int startx, int starty){
    int limit = height * width;
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++)
            mvprintw(starty + i, startx + j, ".");
    }
}

void getCenter(int* x, int* y, int height, int width){
    int wHeight;
    int wWidth;
    getmaxyx(stdscr, wHeight, wWidth);
    *x = wWidth / 2 - width / 2;   
    *y = wHeight / 2 - height / 2;  
}

void addFloor(struct physList * list){
    for (int i = 0; i < W; i++){
        floorB[i] = malloc (sizeof(struct ball));
        floorP[i] = malloc (sizeof(struct physItem));
        constructFloor(floorB[i]);
        createPhysItem(floorB[i], i, H-1, floorP[i]);
        plSubscribe(floorP[i],list);
    }
}
