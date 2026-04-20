#include "ball.h"

float getMassFloat(struct ball *target){
    return target->mass + 1;
}

void constructBall(struct ball *target, float inp_mass){
    constructItem(target, inp_mass, 'o');
}

void constructItem(struct ball *target, float inp_mass, char c){
    *target = (struct ball) {
        .repChar = c,
        .terminatingChar = '\0',
        .mass = inp_mass
    };
}
void constructFloor (struct ball *target){
    constructItem(target, 99, 'x');
}
