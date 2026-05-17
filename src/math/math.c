#include "math.h"
#include <math.h>
/* vectf functions
 * */
struct vectf ve_add(struct vectf v1, struct vectf v2){
    return (struct vectf) {v1.x + v2.x, v1.y + v2.y};
}

struct vectf ve_sub(struct vectf v1, struct vectf v2){
    return (struct vectf) {v1.x - v2.x, v1.y - v2.y};
}

float ve_sumOfSquare(struct vectf v){
    return v.x * v.x + v.y * v.y;
}

float ve_dot(struct vectf v1, struct vectf v2){
    return v1.x * v2.x + v1.y * v2.y;
}


float ve_angle(float sqr1, float sqr2, float dprod){
    float multSqr = sqr1 * sqr2;
    return acosf(dprod * invSqr(multSqr));
}

struct vectf ve_scale(struct vectf v1, float scale){
    return (struct vectf){v1.x * scale, v1.y * scale};
}

struct vectd ve_squash(struct vectf v){
    return (struct vectd) {
        (v.x >= (int) v.x + 0.5f) ? (int) v.x + 1 : (int) v.x,
        (v.y >= (int) v.y + 0.5f) ? (int) v.y + 1 : (int) v.y};
}
/*******************************************************/

/* math functions*/

float invSqr( float number ) {
	int i;
	float halved, res;
	const float threehalfs = 1.5F;

	halved = number * 0.5F;
	res  = number;
	i  = * ( int * ) &res;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	res  = * ( float * ) &i;
	res  = res * ( threehalfs - ( halved * res * res ) );   // 1st iteration

	return res;
}

float inv( float number ) {
    float root = invSqr( number );
    return root * root;
}

float root( float number ) {
    return inv(invSqr(number));
}

float sqr( float number ){
    return number * number;
}
