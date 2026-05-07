#include "math.h"
#include <math.h>
/* vect functions
 * */
struct vect ve_add(struct vect v1, struct vect v2){
    return (struct vect) {v1.x + v2.x, v1.y + v2.y};
}

struct vect ve_sub(struct vect v1, struct vect v2){
    return (struct vect) {v1.x - v2.x, v1.y - v2.y};
}

float ve_sumOfSquare(struct vect v){
    return v.x * v.x + v.y * v.y;
}

float ve_dot(struct vect v1, struct vect v2){
    return v1.x * v2.x + v1.y * v2.y;
}


float ve_angle(float sqr1, float sqr2, float dprod){
    float multSqr = sqr1 * sqr2;
    return acosf(dprod * invSqr(multSqr));
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
