#pragma once

/* vect
 * */
struct vectd {
    int x; int y;
};

struct vectf {
    float x; float y;
};
/****************************************************/
/* vect */
struct vectf ve_add(struct vectf v1, struct vectf v2);
struct vectf ve_sub(struct vectf v1, struct vectf v2);
float ve_sumOfSquare(struct vectf v);
float ve_dot(struct vectf v1, struct vectf v2);
float ve_angle(float sqr1, float sqr2, float dprod);
struct vectf ve_scale(struct vectf v1, float scale);
struct vectd ve_squash(struct vectf);


/* math */
float invSqr( float number );
float inv( float number );
float sqr( float number );
float root( float number );
