#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef enum
{
	NONE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} Motion;
static Motion motion = NONE;
static float cameraX = 0, cameraZ = 0, cameraAngle = 0;
static float rotationSpeed = 1.2, cameraSpeed = 0.5;
static GLuint programId, roomVA, boxVA, glassVA, vertexPosLoc, vertexColLoc, modelMatrixLoc, viewMatrixLoc, projMatrixLoc;
static GLuint programId2, vertexPosLoc2, vertexColLoc2, modelMatrixLoc2, viewMatrixLoc2, projMatrixLoc2;
static Mat4 projMatrix;

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
	modelMatrixLoc = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projMatrix");

	vShader = compileShader("shaders/mvp_xyz_rgba.vsh", GL_VERTEX_SHADER);
	if (!shaderCompiled(vShader))
		return;
	fShader = compileShader("shaders/rgba.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(fShader))
		return;
	programId2 = glCreateProgram();
	glAttachShader(programId2, vShader);
	glAttachShader(programId2, fShader);
	glLinkProgram(programId2);
	vertexPosLoc2 = glGetAttribLocation(programId, "vertexPosition");
	vertexColLoc2 = glGetAttribLocation(programId, "vertexColor");
	modelMatrixLoc2 = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc2 = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc2 = glGetUniformLocation(programId, "projMatrix");
}

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

	glGenVertexArrays(1, &roomVA);
	glBindVertexArray(roomVA);
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

static void drawRoom()
{
	glBindVertexArray(roomVA);
	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, 0);
}

static void createBox(int w, int h, int d)
{
	float x1 = -w / 2, x2 = w / 2;
	float y1 = -h / 2, y2 = h / 2;
	float z1 = -d / 2, z2 = d / 2;

	float boxPos[] = {// Front side
					  x1, y2, z1,
					  x1, y1, z1,
					  x2, y2, z1,
					  x2, y1, z1,
					  // Back side
					  x2, y2, z2,
					  x2, y1, z2,
					  x1, y2, z2,
					  x1, y1, z2,
					  // Left side
					  x1, y2, z2,
					  x1, y1, z2,
					  x1, y2, z1,
					  x1, y1, z1,
					  // Right side
					  x2, y2, z1,
					  x2, y1, z1,
					  x2, y2, z2,
					  x2, y1, z2};

	float boxCol[] = {0, 1, 0.5,
					  0, 1, 0.5,
					  0, 1, 0.5,
					  0, 1, 0.5,

					  1, 0, 0.5,
					  1, 0, 0.5,
					  1, 0, 0.5,
					  1, 0, 0.5,

					  0, 0.5, 1,
					  0, 0.5, 1,
					  0, 0.5, 1,
					  0, 0.5, 1,

					  1, 0.5, 1,
					  1, 0.5, 1,
					  1, 0.5, 1,
					  1, 0.5, 1};
	GLushort boxIndex[] = {0, 1, 2, 3, 0xFFFF,
						   4, 5, 6, 7, 0xFFFF,
						   8, 9, 10, 11, 0xFFFF,
						   12, 13, 14, 15};

	glGenVertexArrays(1, &boxVA);
	glBindVertexArray(boxVA);
	glBindBuffer(GL_ARRAY_BUFFER, 4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxPos), boxPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxCol), boxCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndex), boxIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
}

static void drawBox()
{
	glBindVertexArray(boxVA);
	glBindBuffer(GL_ARRAY_BUFFER, 6);
	glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, 0);
}

static void createGlass(float w, float h)
{
	float x1 = -w / 2, x2 = w / 2;
	float y1 = -h / 2, y2 = h / 2;
	float glassPos[] = {x1, y2, 0,
						x1, y1, 0,
						x2, y2, 0,
						x2, y1, 0};
	float glassColor[] = {0.6, 0.8, 0.9, 0.7,
						  0.6, 0.8, 0.9, 0.7,
						  0.6, 0.8, 0.9, 0.7,
						  0.6, 0.8, 0.9, 0.7};
	glGenVertexArrays(1, &glassVA);
	glBindVertexArray(glassVA);
	glBindBuffer(GL_ARRAY_BUFFER, 6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glassPos), glassPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc2);
	glVertexAttribPointer(vertexPosLoc2, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glassColor), glassColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc2);
	glVertexAttribPointer(vertexColLoc2, 4, GL_FLOAT, 0, 0, 0);
}

static void drawGlass()
{
	glBindVertexArray(glassVA);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void moveForward()
{
	float radians = M_PI * cameraAngle / 180;
	cameraX -= cameraSpeed * sin(radians);
	cameraZ -= cameraSpeed * cos(radians);
}

static void moveBackward()
{
	float radians = M_PI * cameraAngle / 180;
	cameraX += cameraSpeed * sin(radians);
	cameraZ += cameraSpeed * cos(radians);
}

static void rotateLeft()
{
	cameraAngle += rotationSpeed;
}

static void rotateRight()
{
	cameraAngle -= rotationSpeed;
}

static void specialKeyPressed(int code, int x, int y)
{
	switch (code)
	{
	case 101:
		motion = FORWARD;
		break;
	case 103:
		motion = BACKWARD;
		break;
	case 100:
		motion = LEFT;
		break;
	case 102:
		motion = RIGHT;
	}
}

static void specialKeyReleased(int code, int x, int y)
{
	motion = NONE;
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
	setPerspective(&projMatrix, 60, aspect, -1, -100);
	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMatrix.values);
	glUseProgram(programId2);
	glUniformMatrix4fv(projMatrixLoc2, 1, GL_TRUE, projMatrix.values);
}

static void exitFunc(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	reshapeFunc(w, h);
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mat4 modelMat, viewMat;
	mIdentity(&viewMat);
	switch (motion)
	{
	case FORWARD:
		moveForward();
		break;
	case BACKWARD:
		moveBackward();
		break;
	case LEFT:
		rotateLeft();
		break;
	case RIGHT:
		rotateRight();
	}
	rotateY(&viewMat, -cameraAngle);
	translate(&viewMat, -cameraX, 0, -cameraZ);
	glUseProgram(programId);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMat.values);
	glUseProgram(programId2);
	glUniformMatrix4fv(viewMatrixLoc2, 1, GL_TRUE, viewMat.values);

	mIdentity(&modelMat);
	glUseProgram(programId);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	drawRoom();

	static float boxAngle1 = 0;
	translate(&modelMat, 2, 2, -5);
	rotateY(&modelMat, boxAngle1++);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	drawBox();

	static float boxAngle2 = 0;
	mIdentity(&modelMat);
	translate(&modelMat, 2, 2, -10);
	rotateZ(&modelMat, boxAngle2++);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	drawBox();

	glUseProgram(programId2);
	mIdentity(&modelMat);
	translate(&modelMat, 0, 1, -7);
	rotateZ(&modelMat, boxAngle2);
	glUniformMatrix4fv(modelMatrixLoc2, 1, GL_TRUE, modelMat.values);
	drawGlass();

	mIdentity(&modelMat);
	translate(&modelMat, 0, 0, -2);
	glUniformMatrix4fv(modelMatrixLoc2, 1, GL_TRUE, modelMat.values);
	drawGlass();

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);
	glutCreateWindow("Pruebas de profundidad y transparencias");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutReshapeFunc(reshapeFunc);
	glewInit();
	glClearColor(0.05, 0.05, 0.10, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initShaders();
	createRoom(12, 6, 30);
	createBox(2, 3, 2);
	createGlass(8, 3);
	glutMainLoop();
	return 0;
}
