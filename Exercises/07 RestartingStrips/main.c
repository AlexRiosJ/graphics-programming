#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"

#define RESET 0xFF

static Triangle triangles[] = {
    {-1.0, 1.0, 0.0},
    {-1.0, 0.5, 0.0},
    {0.3, 1.0, 0.0},
    {0.3, 0.5, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.5, 0.0},
    {-1.0, 0.5, 0.0},
    {-1.0, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.3, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 0.0},
    {-1.0, -0.5, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, -0.5, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, -0.5, 0.0},
    {-1.0, -0.5, 0.0},
    {-1.0, -1.0, 0.0},
    {0.3, -0.5, 0.0},
    {0.3, -1.0, 0.0},
    {1.0, -0.5, 0.0},
    {1.0, -1.0, 0.0}};

static GLushort indexBuffer[] = {
    0, 1, 2, 3, 4, 5, RESET,
    6, 7, 8, 9, 10, 11, RESET,
    12, 13, 14, 15, 16, 17, RESET,
    18, 19, 20, 21, 22, 23};

static float vertexPos[] = {-1.0, 1.0, -1.0, 0.5,
                            0.3, 1.0, 0.3, 0.5,
                            1.0, 1.0, 1.0, 0.5,

                            -1.0, 0.5, -1.0, 0.0,
                            0.3, 0.5, 0.3, 0.0,
                            1.0, 0.5, 1.0, 0.0,

                            -1.0, 0.0, -1.0, -0.5,
                            0.3, 0.0, 0.3, -0.5,
                            1.0, 0.0, 1.0, -0.5,

                            -1.0, -0.5, -1.0, -1.0,
                            0.3, -0.5, 0.3, -1.0,
                            1.0, -0.5, 1.0, -1.0};

static float vertexCol[] = {1, 1, 0, 1, 1, 0,
                            1, 0, 0, 1, 0, 0,
                            1, 1, 0, 1, 1, 0,

                            1, 0, 1, 1, 0, 1,
                            0, 0, 1, 0, 0, 1,
                            1, 0, 1, 1, 0, 1,

                            0, 1, 1, 0, 1, 1,
                            0, 1, 0, 0, 1, 0,
                            0, 1, 1, 0, 1, 1,

                            1, 1, 1, 1, 1, 1,
                            0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
                            1, 1, 1, 1, 1, 1};

static GLuint va[1];
static GLuint bufferId[3];
static GLuint programId;
static GLuint vertexPosLoc, vertexColLoc;

static void initShaders()
{
    GLuint vShader = compileShader("shaders/colorPosition.vsh", GL_VERTEX_SHADER);
    if (!shaderCompiled(vShader))
        return;

    GLuint fShader = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
    if (!shaderCompiled(fShader))
        return;

    programId = glCreateProgram();
    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);
    glLinkProgram(programId);
    vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    vertexColLoc = glGetAttribLocation(programId, "vertexColor");
}

static void createStrip()
{
    glGenVertexArrays(1, va);
    glBindVertexArray(va[0]);
    glGenBuffers(3, bufferId);

    processArrayBuffer(bufferId[0], triangles, sizeof(triangles), vertexPosLoc, 3, GL_FLOAT);
    processArrayBuffer(bufferId[1], vertexCol, sizeof(vertexCol), vertexColLoc, 3, GL_FLOAT);
    processIndexBuffer(bufferId[2], indexBuffer, sizeof(indexBuffer), RESET);
}

static void drawStrip()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(va[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
    glDrawElements(GL_TRIANGLE_STRIP, 24 + 3, GL_UNSIGNED_SHORT, 0);
    glutSwapBuffers();
}

static void exitFunc(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        glDeleteVertexArrays(1, va);
        exit(0);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    int ww = 800;
    int wh = 600;
    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition((sw - ww) / 2, (sh - wh) / 2);
    glutInitWindowSize(ww, wh);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Reiniciando triangle strips");
    glutDisplayFunc(drawStrip);
    glutKeyboardFunc(exitFunc);
    glewInit();
    initShaders();
    createStrip();
    glClearColor(1, 1, 1, 1.0);
    glutMainLoop();
    return 0;
}
