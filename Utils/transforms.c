#include "transforms.h"
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void translate(Mat4 *csMatrix, float tx, float ty, float tz)
{
    Mat4 trMatrix;
    mIdentity(&trMatrix);
    trMatrix.at[0][3] = tx;
    trMatrix.at[1][3] = ty;
    trMatrix.at[2][3] = tz;
    mMult(csMatrix, trMatrix);
    mPrint(*csMatrix);
}

void rotateX(Mat4 *m, float degrees)
{
}

void rotateY(Mat4 *m, float degrees)
{
}

void rotateZ(Mat4 *csMatrix, float degrees)
{
    Mat4 rotMatrix;
    mIdentity(&rotMatrix);
    float radians = degrees * M_PI / 180;
    rotMatrix.at[0][0] = cos(radians);
    rotMatrix.at[0][1] = -sin(radians);
    rotMatrix.at[1][0] = sin(radians);
    rotMatrix.at[1][1] = cos(radians);

    mMult(csMatrix, rotMatrix);
    mPrint(*csMatrix);
}

void scale(Mat4 *csMatrix, float sx, float sy, float sz)
{
    Mat4 scMatrix;
    mIdentity(&scMatrix);
    scMatrix.at[0][0] = sx;
    scMatrix.at[1][1] = sy;
    scMatrix.at[2][2] = sz;

    mMult(csMatrix, scMatrix);
    mPrint(*csMatrix);
}
