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
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPosLoc);
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);

    Mat4 csMatrix;

    mIdentity(&csMatrix);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1, 0.5, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    translate(&csMatrix, 0.7, 0.0, 0.0);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1, 0, 0.2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    translate(&csMatrix, 0.0, 0.7, 0.0);
    scale(&csMatrix, 0.5, 0.5, 1);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mIdentity(&csMatrix);
    translate(&csMatrix, -0.7, 0, 0);
    rotateZ(&csMatrix, 45);
    glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
    glUniform3f(squareColorLoc, 1, 0.7, 0.8);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mIdentity(&csMatrix);
    translate(&csMatrix, 0, 0.5, 0);

    glUniform3f(squareColorLoc, 1, 1, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform3f(squareColorLoc, 0, 0, 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glutSwapBuffers();
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
    glewInit();
    glClearColor(0.5, 0.5, 0.5, 1.0);
    initShaders();
    init();

    glutMainLoop();
    return 0;
}