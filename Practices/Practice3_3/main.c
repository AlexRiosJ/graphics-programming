#include <GL/glew.h>
#include <GL/freeglut.h>

#include "transforms.h"
#include <math.h>
#include "utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define POINTS 50

GLuint squareColorLoc;
GLuint vertexPosLoc;
GLuint csMatrixLoc;
GLuint programId;
GLuint bufferId;

static void initShaders()
{
    GLuint vShader = compileShader("shaders/vertexPositionCS.vsh", GL_VERTEX_SHADER);
    if (!shaderCompiled(vShader))
        return;

    GLuint fShader = compileShader("shaders/modelColor.fsh", GL_FRAGMENT_SHADER);
    if (!shaderCompiled(fShader))
        return;

    programId = glCreateProgram();

    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);
    glLinkProgram(programId);
    vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    squareColorLoc = glGetUniformLocation(programId, "squareColor");
    csMatrixLoc = glGetUniformLocation(programId, "csMatrix");
}

void genereteHexagonPoints(float r, float array[])
{
    for (int i = 0; i < POINTS * 2; i += 2)
    {
        array[i] = r * cos(((i - 2) / 2) * M_PI / POINTS * 2);
        array[i + 1] = r * sin(((i - 2) / 2) * M_PI / POINTS * 2);
    }
}

static void init()
{
    float pos[POINTS * 2];
    genereteHexagonPoints(0.2, pos);
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &bufferId);
    processArrayBuffer(bufferId, pos, sizeof pos, vertexPosLoc, 2, GL_FLOAT);
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);
    Mat4 csMatrix;
    mIdentity(&csMatrix);
    static float sunAngle = 0, earthAngle = 0, moonAngle = 0, marsAngle;

    rotateZ(&csMatrix, sunAngle);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1.0, 0.8, 0.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    sunAngle -= 0.5;
    pushMatrix(&csMatrix);

    rotateZ(&csMatrix, marsAngle / 5);
    translate(&csMatrix, 0.6, 0, 0);
    rotateZ(&csMatrix, earthAngle);
    scale(&csMatrix, 0.5, 0.5, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.4, 0.8, 1.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    earthAngle += 2.5;

    rotateZ(&csMatrix, moonAngle);
    translate(&csMatrix, 0.5, 0, 0);
    scale(&csMatrix, 0.3, 0.3, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.4, 0.4, 0.4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    moonAngle += 0.5;

    popMatrix(&csMatrix);

    rotateZ(&csMatrix, marsAngle / 5);
    translate(&csMatrix, -0.9, 0, 0);
    rotateZ(&csMatrix, marsAngle);
    scale(&csMatrix, 0.3, 0.3, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.8, 0.4, 0.4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    marsAngle += 1.5;
    pushMatrix(&csMatrix);

    rotateZ(&csMatrix, moonAngle / 3);
    translate(&csMatrix, 0.4, 0, 0);
    scale(&csMatrix, 0.2, 0.2, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.4, 0.4, 0.4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    moonAngle += 0.5;
    popMatrix(&csMatrix);

    rotateZ(&csMatrix, moonAngle / 2);
    translate(&csMatrix, -0.53, 0, 0);
    scale(&csMatrix, 0.3, 0.3, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.4, 0.4, 0.4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS);
    moonAngle += 0.5;

    glutSwapBuffers();
}

static void timerFunc(int id)
{
    glutPostRedisplay();
    glutTimerFunc(10, timerFunc, 1);
}

static void exitFunc(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    int w = 600, h = 600;
    int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
    int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
    glutInitWindowSize(w, h);
    glutInitWindowPosition(x, y);
    glutCreateWindow("Transforms");
    glutDisplayFunc(display);
    glutKeyboardFunc(exitFunc);
    glutTimerFunc(10, timerFunc, 1);
    glewInit();
    glClearColor(0.2, 0.2, 0.3, 1.0);
    initShaders();
    init();

    glutMainLoop();
    return 0;
}