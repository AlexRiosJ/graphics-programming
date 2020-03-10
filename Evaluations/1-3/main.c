#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int change = 0;

static GLuint va[1];
static GLuint bufferId[3];
static float *star_v;
static float *star_c;
static float *star_f;
static int points;
static GLuint vertexPosLoc, vertexColLoc, vertexFactorLoc;
GLuint factorExLoc;
GLuint factorInLoc;
static float factorEx = 1.0;
static float factorIn = 1.0;

GLuint programId;

void keyPressed(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        if (factorEx > 0)
        {
            factorEx -= .1;
        }
    }
    else if (key == 'b')
    {
        if (factorIn > 0)
        {
            factorIn -= .1;
        }
    }
    else if (key == 'c')
    {
        factorEx += .1;
    }
    else if (key == 'd')
    {
        factorIn += .1;
    }

    glutPostRedisplay();
}

void createStar(int numPicos, float extRadio, float intRadio)
{
    points = (numPicos * 2) + 2;
    float angle_step = (2.0 * M_PI) / (points - 2);
    star_v = (float *)calloc(sizeof(float), points * 2);
    star_c = (float *)calloc(sizeof(float), points * 3);
    star_f = (float *)calloc(sizeof(float), points);
    star_v[0] = 0;
    star_v[1] = 0;
    star_c[0] = 1;
    star_c[1] = 1;
    star_c[2] = 1;
    star_f[0] = 0;
    int i = 0;
    int j = 3;
    int k = 1;
    float angle = 0;
    printf("Creating Star...");
    for (i = 2; i < (points * 2) + 1; i += 2)
    {
        if (i % 4 == 0)
        {
            star_v[i] = intRadio * cos(angle);
            star_v[i + 1] = intRadio * sin(angle);
            star_f[k] = 0;
        }
        else
        {
            star_v[i] = extRadio * cos(angle);
            star_v[i + 1] = extRadio * sin(angle);
            star_f[k] = 1;
        }

        star_c[j] = sin(angle) > 0 ? sin(angle) : sin(angle) * -1;
        star_c[j + 1] = cos(angle) > 0 ? cos(angle) : cos(angle) * -1;
        if (i < points + 1)
        {
            star_c[j + 2] = 0;
        }
        else
        {
            star_c[j + 2] = 0.5;
        }
        j += 3;
        k += 1;

        angle += angle_step;
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(va[0]);
    glUseProgram(programId);
    glUniform1f(factorExLoc, factorEx);
    glUniform1f(factorInLoc, factorIn);
    glDrawArrays(GL_TRIANGLE_FAN, 0, points);

    glutSwapBuffers();
}

static void initFigure()
{

    GLuint vshId = compileShader("shaders/starposition.vsh", GL_VERTEX_SHADER);
    GLuint fshId = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);

    programId = glCreateProgram();
    glAttachShader(programId, vshId);
    glAttachShader(programId, fshId);
    glLinkProgram(programId);

    vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    vertexColLoc = glGetAttribLocation(programId, "vertexColor");
    vertexFactorLoc = glGetAttribLocation(programId, "vertexFactor");

    glGenVertexArrays(1, va);
    glBindVertexArray(va[0]);
    glGenBuffers(3, bufferId);

    glBindVertexArray(va[0]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
    glBufferData(GL_ARRAY_BUFFER, points * 2 * sizeof(float), star_v, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPosLoc);

    glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
    glBufferData(GL_ARRAY_BUFFER, points * 3 * sizeof(float), star_c, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(vertexColLoc);

    glBindBuffer(GL_ARRAY_BUFFER, bufferId[2]);
    glBufferData(GL_ARRAY_BUFFER, points * sizeof(float), star_f, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexFactorLoc, 1, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(vertexFactorLoc);

    factorExLoc = glGetUniformLocation(programId, "factorEx");
    factorInLoc = glGetUniformLocation(programId, "factorIn");
}

int main(int argc, char **arg)
{
    glutInit(&argc, arg);
    int w = 600;
    int h = 600;
    glutInitWindowSize(w, h);

    int psx = glutGet(GLUT_SCREEN_WIDTH);
    int psy = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition((psx - w) / 2, (psy - h) / 2);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutCreateWindow("Star");
    createStar(40, 0.9, 0.4);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);

    glewInit();

    initFigure();

    glClearColor(0.1, 0.1, 0.2, 1.0);

    glutMainLoop();
    while (1)
    {
        glutMainLoopEvent();
    }
    return 0;
}