#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"

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

static void init()
{
    float pos[] = {-0.2, 0.2, -0.2, -0.2, 0.2, 0.2, 0.2, -0.2};
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
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.8, 0.6, 0.2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    static float angle1 = 0, angle2 = 0, angle3 = 0;
    rotateZ(&csMatrix, angle1);
    translate(&csMatrix, 0.6, 0.0, 0.0);
    rotateZ(&csMatrix, angle2);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.5, 0.1, 0.6);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    translate(&csMatrix, 0.0, 0.6, 0.0);
    rotateZ(&csMatrix, -angle3);
    scale(&csMatrix, 0.5, 0.5, 1.0);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1.0, 0.0, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    angle1 += 0.8;
    angle2 += 2.0;
    angle3 += 20.0;

    static float angle = -45;

    mIdentity(&csMatrix);
    translate(&csMatrix, -0.6, 0.0, 0.0);
    rotateZ(&csMatrix, -angle);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1.0, 0.5, 0.8);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mIdentity(&csMatrix);
    translate(&csMatrix, 0.0, 0.6, 0.0);
    rotateZ(&csMatrix, angle);
    scale(&csMatrix, 1.2, 0.7, 1.0);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.9, 1.0, 0.4);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mIdentity(&csMatrix);
    translate(&csMatrix, 0.0, -0.6, 0.0);
    scale(&csMatrix, 1.2, 0.7, 1.0);
    rotateZ(&csMatrix, angle);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 0.2, 0.8, 1.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    angle += 0.5;
    if (angle >= 360.0)
        angle -= 360.0;

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
    glClearColor(0.5, 0.5, 0.5, 1.0);
    initShaders();
    init();

    glutMainLoop();
    return 0;
}