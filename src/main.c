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
#include "phys/obj.h"
#include "math/math.h"
#include "const.h"

#define HIGH_BIRD "‾"
#define MID_BIRD "-"
#define LOW_BIRD "_"

void getCenter(int*, int*);
void drawGrid(int startx, int starty);
float getTime();

// void *input_listener(void * args){
    
// }
// pthread_create();
struct timespec ts;

int main(){
    setlocale(LC_ALL, "C.UTF-8");

    struct physList *pl = pl_create();
    int topLeftX, topLeftY;

    struct physItem *bird = pl_subscribe("e", W/2, H/2, pl, 0.5f, true, 1.f, (struct vectf){0, 2});
    /* ***********************************************/

    // ncurses window setup
    initscr();
    
    // default color variables
        // every color represented by descriptor
        // every pair of foreground and background also represented by descriptor
    // 1. setup colors
    start_color();
    short fg_color;
    short bg_color;
    short grid_color = COLOR_WHITE;
    if (can_change_color()) {
        init_color(COLOR_WHITE, 500, 500, 500);
    }
    pair_content(0, &fg_color, &bg_color);

    // 2. setup pairs
    short bird_color = 1;
    short wall_color = 2;
    init_pair(bird_color, COLOR_GREEN, bg_color);
    init_pair(wall_color, COLOR_RED, bg_color);


    cbreak();
    noecho();
    nodelay(stdscr, true);
    curs_set(0); // hides cursor
    getCenter(&topLeftX, &topLeftY);
    bool clip = true;
    /* ***********************************************/
    
    // timer setup
    float t1;
    float t2;

    t1 = getTime();
    /* ***********************************************/

    // GAME LOOP
    while (1){
        // pre-iter processing
        // jump
        if (getch() != ERR){
            if (pi_isFalling(bird)) pi_resetVelocity(bird);
            pi_timedForce(bird, 0.1f, (struct vectf) {0,-50});
            // log
            log_item("PRESSED!: " , bird);
        }

        t2 = getTime();
        phys_iterate(pl, topLeftX, topLeftY, t2-t1);
        t1 = getTime();

        // DRAW
        erase();
        drawGrid(topLeftX, topLeftY);

        // draw physItems
        struct physItem *curr = pl->head;
        while (curr != NULL){
            if ( clip && !(curr->coords.x <= W-1 && curr->coords.x >= 0 && curr -> coords.y <= H-1 && curr -> coords.y >= 0) ) {curr = curr->next; continue;}
            
            // draw bird
            if (curr->obj[0] == 'e'){
                attron(COLOR_PAIR(bird_color));

                // n and HIGH: n inc. to n.33333334f exc.
                // n and mid: n.33333334f inc. to n.66666668f exc.
                // n and low: n.66666668f inc. to 1 exc.
                //
                struct vectd coords = {(int)curr->coords.x, (int)curr->coords.y};
                float height = (curr->coords.y) - coords.y;
                if (height < .33333334f) {
                    mvprintw(topLeftY+ (coords.y),
                        topLeftX+ (coords.x),
                        HIGH_BIRD);
                }
                else if (height < .66666668f) {
                    mvprintw(topLeftY+ (coords.y),
                        topLeftX+ (coords.x),
                        MID_BIRD);
                }
                else {
                    mvprintw(topLeftY+ (coords.y),
                        topLeftX+ (coords.x),
                        LOW_BIRD);
                }

                attroff(COLOR_PAIR(bird_color));

            } else if (curr->obj[0] == '|'){

                attron(COLOR_PAIR(wall_color));

                struct vectd coords = ve_squash(curr->coords);
                mvprintw(topLeftY+ (coords.y),
                    topLeftX+ (coords.x),
                    pi_getItem(curr));

                attroff(COLOR_PAIR(wall_color));
            }

            curr = curr->next;
        }
        refresh();
        // DRAW END
    }
    // GAME LOOP END
    
    // destructors
    endwin();
    pl_destroy(pl);
    /* ***********************************************/

    return 0;
}

// void addWall(struct physList *pl, int center, int radius){
//     pl_subscribe("|", i, pl, 1.f, true, 1.f, (struct vectf){0, 1});
// }

float getTime(){
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9f;
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
