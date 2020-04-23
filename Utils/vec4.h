#ifndef VEC4_H_
#define VEC4_H_

#include "mat4.h"

typedef struct
{
	float x, y, z, w;
} Vec4;

float vec4_magnitude(Vec4 *);

void vec4_normalize(Vec4 *);

void multiply(Mat4, Vec4, Vec4 *);

#endif /* VEC4_H_ */
