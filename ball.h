struct ball{
    char repChar,
    char terminatingChar,
    float mass;
};

float getMassFloat(struct ball *);
void constructBall(struct ball *target, float inp_mass);
