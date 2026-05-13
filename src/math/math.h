#pragma once

/* vect
 * */
struct vect {
    int x; int y;
};
/****************************************************/
/* vect */
struct vect ve_add(struct vect v1, struct vect v2);
struct vect ve_sub(struct vect v1, struct vect v2);
float ve_sumOfSquare(struct vect v);
float ve_dot(struct vect v1, struct vect v2);
float ve_angle(float sqr1, float sqr2, float dprod);
struct vect ve_scale(struct vect v1, float scale);


/* math */
float invSqr( float number );
float inv( float number );
float sqr( float number );
float root( float number );
