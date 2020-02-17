#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "mat4.h"

void translate(Mat4 *, float, float, float);

void rotateX(Mat4 *, float);

void rotateY(Mat4 *, float);

void rotateZ(Mat4 *, float);

void scale(Mat4 *, float, float, float);

#endif /* TRANSFORMS_H_ */
