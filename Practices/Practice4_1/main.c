#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include "utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIDES_N 5
#define LAYERS_N 4

#define RESET 0xFFFF

static GLuint va[1];
static GLuint bufferId[3];
static GLuint programId;
static GLuint vertexPosLoc, vertexColLoc;

int nPoints = (SIDES_N + 1) * (LAYERS_N + 1);

void keyPressed(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
}

void generatePoints(float width, float array[], float color[])
{
	float angle = M_PI / 4;
	float radius = 1;

	srand(time(NULL));
	float r = rand() / (RAND_MAX * 1.0);
	float g = rand() / (RAND_MAX * 1.0);
	float b = rand() / (RAND_MAX * 1.0);

	for (int i = 0; i < LAYERS_N; i++)
	{
		for (int j = 0; j < SIDES_N + 1; j++)
		{
			int currentIndex = (i * ((SIDES_N + 1) * 4)) + (j * 4);
			array[currentIndex] = cos(angle) * radius;
			array[currentIndex + 1] = sin(angle) * radius;
			array[currentIndex + 2] = cos(angle) * (radius - width);
			array[currentIndex + 3] = sin(angle) * (radius - width);
			angle += (3 * M_PI / 2) / SIDES_N;

			int colorIndex = (i * ((SIDES_N + 1) * 6)) + (j * 6);
			color[colorIndex] = r;
			color[colorIndex + 1] = g;
			color[colorIndex + 2] = b;
			color[colorIndex + 3] = r;
			color[colorIndex + 4] = g;
			color[colorIndex + 5] = b;
			b = rand() / (RAND_MAX * 1.0);
		}
		angle = M_PI / 4;
		radius -= width;
		r = rand() / (RAND_MAX * 1.0);
		g = rand() / (RAND_MAX * 1.0);
	}
}

void generateIndexBuffer(GLushort array[])
{
	int index = 0;
	int counter = 0;
	int j = 0;
	for (int i = 0; i < LAYERS_N * (((SIDES_N + 1) * 2) + 1); i++)
	{
		array[i] = j;
		counter++;
		if (counter == (((SIDES_N + 1) * 2) + 1))
		{
			counter = 0;
			array[i] = RESET;
			continue;
		}
		j++;
	}
}

void initShaders()
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

void init()
{
	float vertexes[(SIDES_N + 1) * 2 * LAYERS_N * 2];
	float color[(SIDES_N + 1) * 2 * LAYERS_N * 3];
	GLushort indexBuffer[LAYERS_N * (((SIDES_N + 1) * 2) + 1)];
	float width = 0.15;

	generatePoints(width, vertexes, color);
	generateIndexBuffer(indexBuffer);

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);

	processArrayBuffer(bufferId[0], vertexes, sizeof(vertexes), vertexPosLoc, 2, GL_FLOAT);
	processArrayBuffer(bufferId[1], color, sizeof(color), vertexColLoc, 3, GL_FLOAT);
	processIndexBuffer(bufferId[2], indexBuffer, sizeof(indexBuffer), RESET);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawElements(GL_TRIANGLE_STRIP, LAYERS_N * (((SIDES_N + 1) * 2) + 1), GL_UNSIGNED_SHORT, 0);
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

	glClearColor(1, 1, 1, 1.0);
	initShaders();
	init();

	glutMainLoop();
	return 0;
}
