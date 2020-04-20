#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "utils.h"

typedef struct strCylinder *Cylinder;

Cylinder cylinder_create(float length, float bottomRadius, float topRadius, int slices, int stacks, Vertex bottomColor, Vertex topColor);
void cylinder_bind(Cylinder cylinder, GLuint vertexPosLoc, GLuint vertexColLoc, GLuint vertexNormalLoc);
void cylinder_draw(Cylinder cylinder);

#endif /* CYLINDER_H_ */
