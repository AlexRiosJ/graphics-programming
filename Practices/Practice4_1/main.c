#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIDES_N 50
#define LAYERS_N 9

#define RESET 0xFF

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
	int sideCount = 0;
	float angle = M_PI / 4;
	float radius = 1;
	float r, g, b;
	r = rand() / (RAND_MAX * 1.0);
	g = rand() / (RAND_MAX * 1.0);
	for (int i = 0, j = 0; i < nPoints * 2; i += 2, j += 3)
	{
		array[i] = cos(angle) * radius;
		array[i + 1] = sin(angle) * radius;
		angle += (3 * M_PI / 2) / SIDES_N;
		sideCount++;
		b = rand() / (RAND_MAX * 1.0);
		if (sideCount > SIDES_N)
		{
			r = rand() / (RAND_MAX * 1.0);
			g = rand() / (RAND_MAX * 1.0);
			angle = M_PI / 4;
			radius -= width;
			sideCount = 0;
		}
		color[j] = r;
		color[j + 1] = g;
		color[j + 2] = b;
		printf("%.2f, %.2f, %.2f\n", color[j], color[j + 1], color[j + 2]);
	}
}

void generateIndexBuffer(GLushort array[])
{
	int nSquares = SIDES_N * LAYERS_N;
	int sideCount = 0;
	for (int i = 0, j = 0; i < nSquares * 5; i += 5, j++)
	{
		if (sideCount == SIDES_N)
		{
			sideCount = 0;
			// printf("NEW LAYER ------- \n");
			i -= 5;
		}
		else
		{
			array[i] = SIDES_N + 1 + j;
			array[i + 1] = j;
			array[i + 2] = SIDES_N + 2 + j;
			array[i + 3] = j + 1;
			array[i + 4] = RESET;
			sideCount++;
			// printf("%d, %d, %d, %d, %d, %d, %X\n", i, j, array[i], array[i + 1], array[i + 2], array[i + 3], array[i + 4]);
		}
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
	float vertexes[nPoints * 2];
	float width = 0.1;
	float color[nPoints * 3];
	generatePoints(width, vertexes, color);

	GLushort indexBuffer[SIDES_N * LAYERS_N * 5];
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
	glDrawElements(GL_TRIANGLE_STRIP, SIDES_N * LAYERS_N * 5, GL_UNSIGNED_SHORT, 0);
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
	initShaders();
	init();

	glutMainLoop();
	return 0;
}
