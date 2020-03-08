#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include "utils.h"

#define RESET 0xFFFF
#define ROWS 8
#define COLUMNS 8

static GLuint va[1];
static GLuint bufferId[3];
static GLuint programId;
static GLuint vertexPosLoc, vertexColLoc;

void keyPressed(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
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

void init(int rows, int columns)
{
	float vertexes[(rows + 1) * (columns + 1) * 2];
	float color[(rows + 1) * (columns + 1) * 3];
	GLushort indexBuffer[((columns + 1) * 2 + 1) * rows];
	float dx = (float)2 / columns;
	float dy = (float)2 / rows;
	srand(time(NULL));

	// Generate points and colors
	for (int i = 0; i < rows + 1; i++)
	{
		for (int j = 0; j < columns + 1; j++)
		{
			int index = i * (columns + 1) + j;
			vertexes[index * 2] = -1 + dx * j;
			vertexes[index * 2 + 1] = -1 + dy * i;
			color[index * 3] = (float)rand() / RAND_MAX;
			color[index * 3 + 1] = (float)rand() / RAND_MAX;
			color[index * 3 + 2] = (float)rand() / RAND_MAX;
		}
	}

	// Generate index buffer
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < (columns + 1) * 2 + 1; j++)
		{
			int index = i * ((columns + 1) * 2 + 1) + j;
			if (j == (columns + 1) * 2)
			{
				indexBuffer[index] = RESET;
			}
			else
			{
				int num = i * (columns + 1) + (j / 2);
				indexBuffer[index] = j % 2 == 0 ? num : num + columns + 1;
			}
		}
	}

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);
	glPrimitiveRestartIndex(RESET);
	glEnable(GL_PRIMITIVE_RESTART);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawElements(GL_TRIANGLE_STRIP, ROWS * (((COLUMNS + 1) * 2) + 1), GL_UNSIGNED_SHORT, 0);
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

	init(ROWS, COLUMNS);

	glutMainLoop();
	return 0;
}
