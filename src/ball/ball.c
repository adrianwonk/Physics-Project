#include "ball.h"
#include <stdlib.h>

struct ball * constructItem(float inp_mass, char c){
    struct ball *tmp = malloc(sizeof(struct ball));
    *tmp = (struct ball) {
        .repChar = c,
        .terminatingChar = '\0',
        .mass = inp_mass
    };

    return tmp;
}
