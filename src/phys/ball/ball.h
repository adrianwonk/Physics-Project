#pragma once
struct ball{
    char repChar;
    char terminatingChar;
    float mass;
};

struct ball * constructItem(float inp_mass, char c);
