#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include <time.h>
#include <stdlib.h>

static GLuint programId, vertexPositionLoc, vertexColorLoc;
static GLuint vaId[1], bufferId[3];

static void initShaders()
{
    GLuint vShaderId = compileShader("shaders/colorPosition.vsh", GL_VERTEX_SHADER);
    if (!shaderCompiled(vShaderId))
        return;

    GLuint fShaderId = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
    if (!shaderCompiled(fShaderId))
        return;

    programId = glCreateProgram();
    glAttachShader(programId, vShaderId);
    glAttachShader(programId, fShaderId);
    glLinkProgram(programId);

    vertexPositionLoc = glGetAttribLocation(programId, "vertexPosition");
    vertexColorLoc = glGetAttribLocation(programId, "vertexColor");
}

static float random()
{
    return (float)rand() / RAND_MAX;
}

static void createShape()
{

    GLushort indexes[] = {
        0, 1, 3,
        3, 5, 7,
        7, 9, 11,
        0, 3, 2,
        2, 3, 6,
        3, 7, 6,
        6, 7, 10,
        7, 11, 10,
        2, 6, 4,
        6, 10, 8};

    float positions[] = {
        -0.75, -0.25,
        -0.50, -0.75,
        -0.50, 0.25,
        -0.25, -0.25,
        -0.25, 0.75,
        0.00, -0.75,
        0.00, 0.25,
        0.25, -0.25,
        0.25, 0.75,
        0.50, -0.75,
        0.50, 0.25,
        0.75, -0.25};

    float colors[] = {
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random(),
        1.0, random(), random()};

    // Crear vertex array y vincularlo
    glGenVertexArrays(1, vaId);
    glBindVertexArray(vaId[0]);
    // Crear los tres búfers. Declarar el arreglo de búfers como global.
    glGenBuffers(3, bufferId);
    // Vincular cada búfer con los datos de los arreglos
    // Llenar búfer de posiciones
    glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPositionLoc);
    // Llenar búfer de colores
    glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexColorLoc);
    // Llenar buffer de posición
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
}

static void displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(vaId[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
    glDrawElements(GL_LINE_STRIP, 30, GL_UNSIGNED_SHORT, 0);
    glutSwapBuffers();
}

static void exitFunc(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    srand(time(NULL));
    glutInit(&argc, argv);
    int ww = 800;
    int wh = 600;
    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition((sw - ww) / 2, (sh - wh) / 2);
    glutInitWindowSize(ww, wh);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Arreglo de índices");
    glutKeyboardFunc(exitFunc);
    glutDisplayFunc(displayFunc);
    glewInit();
    initShaders();
    createShape();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutMainLoop();
    return 0;
}
