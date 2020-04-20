#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"
#include "sphere.h"
#include "cylinder.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
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

Sphere sphere;
Cylinder cylinder;

static Mat4 projectionMatrix, modelMatrix, viewMatrix;
static GLuint programId, vertexPosLoc, vertexColLoc, vertexNormalLoc, modelMatrixLoc, viewMatrixLoc, projMatrixLoc;
static GLuint ambientLightLoc, diffuseLightLoc, lightPositionLoc, materialALoc, materialDLoc, materialSLoc, exponentLoc, cameraLoc;

static float angleY = 0, angleZ = 0;

Motion motion = NONE;
static float cameraX = 0;
static float cameraZ = 3;
static float cameraAngle = 0;
static float cameraSpeed = 0.1;
static float rotationSpeed = 2;

static float ambientLight[] = {1, 1, 1};
static float materialA[] = {0.5, 0.5, 0.5};
static float diffuseLight[] = {1, 1, 1};
static float lightPosition[] = {0, 0, 10};
static float materialD[] = {0.5, 0.5, 0.5};
static float materialS[] = {0.5, 0.5, 0.5};
static float exponent = 16;

static void initShaders()
{
	GLuint vShader = compileShader("shaders/projection-gouraud.vsh", GL_VERTEX_SHADER);
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
	modelMatrixLoc = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projectionMatrix");

	ambientLightLoc = glGetUniformLocation(programId, "ambientLight");
	diffuseLightLoc = glGetUniformLocation(programId, "diffuseLight");
	lightPositionLoc = glGetUniformLocation(programId, "lightPosition");
	materialALoc = glGetUniformLocation(programId, "materialA");
	materialDLoc = glGetUniformLocation(programId, "materialD");
	materialSLoc = glGetUniformLocation(programId, "materialS");
	exponentLoc = glGetUniformLocation(programId, "exponent");
	cameraLoc = glGetUniformLocation(programId, "camera");

	glUniform3fv(ambientLightLoc, 1, ambientLight);
	glUniform3fv(diffuseLightLoc, 1, diffuseLight);
	glUniform3fv(lightPositionLoc, 1, lightPosition);
	glUniform3fv(materialALoc, 1, materialA);
	glUniform3fv(materialDLoc, 1, materialD);
	glUniform3fv(materialSLoc, 1, materialS);
	glUniform1f(exponentLoc, exponent);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	mIdentity(&modelMatrix);
	mIdentity(&viewMatrix);
	glUseProgram(programId);

	glUniform3f(cameraLoc, cameraX, 0, cameraZ);

	rotateY(&viewMatrix, -cameraAngle);
	translate(&viewMatrix, -cameraX, 0, -cameraZ);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix.values);

	static float angle = -45;

	rotateY(&modelMatrix, 30);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	sphere_draw(sphere);

	mIdentity(&modelMatrix);
	rotateY(&modelMatrix, angle);
	translate(&modelMatrix, 10, 0, 0);
	rotateX(&modelMatrix, angle);
	rotateZ(&modelMatrix, angle);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	cylinder_draw(cylinder);

	angle += 1;
	if (angle >= 360.0)
		angle -= 360.0;

	glutSwapBuffers();
}

static void timerFunc(int id)
{
	glutTimerFunc(10, timerFunc, id);
	glutPostRedisplay();
}

static void reshapeFunc(int w, int h)
{
	glViewport(0, 0, w, h);
	float aspect = (float)w / h;
	setPerspective(&projectionMatrix, 70, aspect, -0.1, -2000);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projectionMatrix.values);
}

static void exitFunc(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Sphere and Cylinder");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutReshapeFunc(reshapeFunc);
	glewInit();
	initShaders();

	Vertex sphereColor1 = {0.8, 0.3, 0.8};
	sphere = sphere_create(1.5, 40, 40, sphereColor1);
	sphere_bind(sphere, vertexPosLoc, vertexColLoc, vertexNormalLoc);

	Vertex cylinderBottomColor = {1, 0, 0};
	Vertex cylinderTopColor = {0, 0, 1};
	cylinder = cylinder_create(6, 0.5, 1.5, 30, 30, cylinderBottomColor, cylinderTopColor);
	cylinder_bind(cylinder, vertexPosLoc, vertexColLoc, vertexNormalLoc);

	glClearColor(0, 0, 0, 1.0);
	glutMainLoop();
	return 0;
}
