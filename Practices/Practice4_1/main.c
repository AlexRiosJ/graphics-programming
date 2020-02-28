#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIDES_N 5
#define LAYERS_N 4

#define RESET 0xFF

static GLuint va[1];
static GLuint bufferId[2];
static GLuint programId;

int nPoints = (SIDES_N + 1) * (LAYERS_N + 1);

void keyPressed(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
}

void generatePoints(float width, float array[])
{
	int sideCount = 0;
	float angle = M_PI / 4;
	float radius = 1;
	for (int i = 0; i < nPoints * 2; i += 2)
	{
		array[i] = cos(angle) * radius;
		array[i + 1] = sin(angle) * radius;
		angle += (3 * M_PI / 2) / SIDES_N;
		sideCount ++;
		if (sideCount > SIDES_N)
		{
			angle = M_PI / 4;
			radius -= width;
			sideCount = 0;
		}
	}
}

void generateIndexBuffer(GLushort array[])
{
	int nSquares = SIDES_N * LAYERS_N;
	for(int i = 0, j = 0; i < nSquares * 5; i += 5, j ++)
	{
		array[i] = SIDES_N + 1 + j;
		array[i + 1] = j;
		array[i + 2] = SIDES_N + 2 + j;
		array[i + 3] = j + 1;
		array[i + 4] = RESET;
		printf("%d, %d, %d, %d, %X\n", array[i], array[i + 1], array[i + 2], array[i + 3], array[i + 4]);
	}
}

void init()
{
	GLuint fshId = compileShader("./shaders/color.fsh", GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
	glAttachShader(programId, fshId);
	glLinkProgram(programId);

    glGenVertexArrays(1, va);
    glBindVertexArray(va[0]);
	glGenBuffers(2, bufferId);

	float vertexes[nPoints * 2];
	float width = 0.15;
	generatePoints(width, vertexes);

	GLushort indexBuffer[SIDES_N * LAYERS_N * 5];
	generateIndexBuffer(indexBuffer);

    processArrayBuffer(bufferId[0], vertexes, sizeof(vertexes), 0, 2, GL_FLOAT);
    processIndexBuffer(bufferId[1], indexBuffer, sizeof(indexBuffer), RESET);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(va[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
    glDrawElements(GL_TRIANGLE_STRIP, 100 + 3, GL_UNSIGNED_SHORT, 0);
    glutSwapBuffers();
}

static void timerFunc(int id)
{
	glutTimerFunc(5, timerFunc, 1);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutCreateWindow("Practice 4.1");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);
	glewInit();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
