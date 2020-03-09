#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"
#include <stdio.h>

static GLuint programId, va[1], vertexPosLoc, vertexColLoc, csMatrixLoc, projMatrixLoc;
static Mat4 projMatrix;
static GLboolean usePerspective = GL_TRUE;

static void initShaders()
{
	GLuint vShader = compileShader("shaders/projection.vsh", GL_VERTEX_SHADER);
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
	csMatrixLoc = glGetUniformLocation(programId, "csMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projMatrix");

	glUseProgram(programId);
	//	glEnable(GL_CULL_FACE);
	//	glFrontFace(GL_CW);
}

// Crea un cuarto de (w x h x d) centrado en el origen
static void createRoom(int w, int h, int d)
{
	float x1 = -w / 2, x2 = w / 2;
	float y1 = -h / 2, y2 = h / 2;
	float z1 = -d / 2, z2 = d / 2;
	float roomPos[] = {// Front (far) wall
					   x1, y1, z1,
					   x1, y2, z1,
					   x2, y1, z1,
					   x2, y2, z1,
					   // Back (near) wall
					   x2, y1, z2,
					   x2, y2, z2,
					   x1, y1, z2,
					   x1, y2, z2,
					   // Left wall
					   x1, y1, z2,
					   x1, y2, z2,
					   x1, y1, z1,
					   x1, y2, z1,
					   // Right wall
					   x2, y1, z1,
					   x2, y2, z1,
					   x2, y1, z2,
					   x2, y2, z2,
					   // Floor
					   x1, y1, z1,
					   x2, y1, z1,
					   x1, y1, z2,
					   x2, y1, z2,
					   // Ceiling
					   x2, y2, z1,
					   x1, y2, z1,
					   x2, y2, z2,
					   x1, y2, z2};

	float roomCol[] = {0, 1, 0,
					   0, 1, 0,
					   0, 1, 0,
					   0, 1, 0,

					   1, 0, 0,
					   1, 0, 0,
					   1, 0, 0,
					   1, 0, 0,

					   0, 0, 1,
					   0, 0, 1,
					   0, 0, 1,
					   0, 0, 1,

					   1, 0, 1,
					   1, 0, 1,
					   1, 0, 1,
					   1, 0, 1,

					   1, 1, 0,
					   1, 1, 0,
					   1, 1, 0,
					   1, 1, 0,

					   0, 1, 1,
					   0, 1, 1,
					   0, 1, 1,
					   0, 1, 1};

	GLushort roomIndex[] = {0, 1, 2, 3, 0xFFFF,
							4, 5, 6, 7, 0xFFFF,
							8, 9, 10, 11, 0xFFFF,
							12, 13, 14, 15, 0xFFFF,
							16, 17, 18, 19, 0xFFFF,
							20, 21, 22, 23};

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roomPos), roomPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roomCol), roomCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roomIndex), roomIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
}

static void createSquare()
{
	float squarePos[] = {-2, -1, 3,
						 -2, 1, 3,
						 2, -1, 3,
						 2, 1, 3,

						 2, -1, 3,
						 2, 1, 3,
						 2, -1, -3,
						 2, 2, -3,

						 2, 1, 3,
						 -2, 1, 3,
						 2, 2, -3,
						 -2, 2, -3,

						 -2, -1, -3,
						 -2, 2, -3,
						 -2, -1, 3,
						 -2, 1, 3,

						 -2, -1, -3,
						 -2, -1, 3,
						 2, -1, -3,
						 2, -1, 3,

						 -2, -1, -3,
						 2, -1, -3,
						 -2, 2, -3,
						 2, 2, -3};

	float squareCol[] = {0, 1, 0,
						 0, 1, 0,
						 0, 1, 0,
						 0, 1, 0,

						 1, 0, 0,
						 1, 0, 0,
						 1, 0, 0,
						 1, 0, 0,

						 0, 0, 1,
						 0, 0, 1,
						 0, 0, 1,
						 0, 0, 1,

						 1, 0, 1,
						 1, 0, 1,
						 1, 0, 1,
						 1, 0, 1,

						 0, 1, 1,
						 0, 1, 1,
						 0, 1, 1,
						 0, 1, 1,

						 1, 1, 0,
						 1, 1, 0,
						 1, 1, 0,
						 1, 1, 0};

	GLushort squareIndex[] = {0, 1, 2, 3, 0xFFFF,
							  4, 5, 6, 7, 0xFFFF,
							  8, 9, 10, 11, 0xFFFF,
							  12, 13, 14, 15, 0xFFFF,
							  16, 17, 18, 19, 0xFFFF,
							  20, 21, 22, 23};

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squarePos), squarePos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareCol), squareCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndex), squareIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
}

static float angleY = 0, angleZ = 0;

static void display()
{
	Mat4 csMat;
	mIdentity(&csMat);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	if (usePerspective)
		translate(&csMat, 0, 0, -3);
	//	rotateY(&csMat, angleY += 1);
	//	rotateX(&csMat, angle ++);
	//	rotateZ(&csMat, angleZ += 2.3);
	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMat.values);
	glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
}

static void timerFunc(int id)
{
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

static void reshapeFunc(int w, int h)
{
	glViewport(0, 0, w, h);
	float aspect = (float)w / h;
	if (usePerspective)
	{
		setPerspective(&projMatrix, 60, aspect, -1, -20);
	}
	else
	{
		if (aspect >= 1.0)
			setOrtho(&projMatrix, -6 * aspect, 6 * aspect, -6, 6, -6, 6);
		else
			setOrtho(&projMatrix, -6, 6, -6 / aspect, 6 / aspect, -6, 6);
	}
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMatrix.values);
}

static void exitFunc(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		glDeleteVertexArrays(1, va);
		exit(0);
	}
	if (key == 13)
	{
		usePerspective = !usePerspective;
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
		reshapeFunc(w, h);
	}
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Mi primera experiencia con coordenadas 3D");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutReshapeFunc(reshapeFunc);
	glewInit();
	initShaders();
	createRoom(6, 4, 15);
	glClearColor(0.05, 0.05, 0.10, 1.0);
	glutMainLoop();
	return 0;
}
