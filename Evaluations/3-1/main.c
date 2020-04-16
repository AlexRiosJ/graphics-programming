#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"
#include "sphere.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Sphere sphere1, sphere2;

static GLuint programId, va[1], bufferId[2], vertexPosLoc, vertexColLoc, vertexNormalLoc, modelMatrixLoc, viewMatrixLoc, projMatrixLoc;
static Mat4 projMatrix;
static GLboolean usePerspective = GL_TRUE;
static float angleY = 0, angleZ = 0;
typedef enum
{
	NONE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} Motion;
Motion motion = NONE;
static float cameraX = 0;
static float cameraZ = 3;
static float cameraAngle = 0;
static float cameraSpeed = 0.1;
static float rotationSpeed = 2;

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
	glUseProgram(programId);
	vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
	vertexColLoc = glGetAttribLocation(programId, "vertexColor");
	vertexNormalLoc = glGetAttribLocation(programId, "vertexNormal");

	sphere_bind(sphere1, vertexPosLoc, vertexColLoc, vertexNormalLoc);
	sphere_bind(sphere2, vertexPosLoc, vertexColLoc, vertexNormalLoc);

	modelMatrixLoc = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projMatrix");

	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	//	glFrontFace(GL_CW);
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

static void display()
{
	Mat4 modelMat;
	Mat4 viewMat;
	mIdentity(&modelMat);
	mIdentity(&viewMat);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);

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
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMat.values);

	static float angle = -45;

	pushMatrix(&modelMat);
	rotateX(&modelMat, angle);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	sphere_draw(sphere1);

	popMatrix(&modelMat);
	rotateY(&modelMat, angle);
	translate(&modelMat, 0, 0, 2.5);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	sphere_draw(sphere2);

	angle += 1;
	if (angle >= 360.0)
		angle -= 360.0;

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
		setPerspective(&projMatrix, 80, aspect, -0.1, -2000);
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

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Sphere");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutReshapeFunc(reshapeFunc);
	glewInit();

	Vertex sphereColor1 = {1, 0.5, 0.5};
	sphere1 = sphere_create(2, 40, 40, sphereColor1);
	Vertex sphereColor2 = {0.6, 0.5, 1};
	sphere2 = sphere_create(0.3, 40, 40, sphereColor2);
	initShaders();

	glClearColor(1, 1, 1, 1.0);
	glutMainLoop();
	return 0;
}
