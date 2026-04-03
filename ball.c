#include "ball.h"

float getMassFloat(struct ball *target){
    return target->mass + 1;
}

void constructBall(struct ball *target, float inp_mass){
    *target = (struct ball) {
        repChar = 'o',
        terminatingChar = '\0',
        mass = inp_mass;
    };
}
