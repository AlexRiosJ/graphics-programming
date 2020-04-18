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

typedef enum
{
	NONE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} Motion;

Sphere sphere;
Vertex spherePosition = {0, 0, 0};
Vertex sphereVelocity = {0.05, 0.05, 0.07};
static const float SPHERE_RADIUS = 0.5;

static const int ROOM_WIDTH = 15;
static const int ROOM_HEIGHT = 10;
static const int ROOM_DEPTH = 20;

GLuint roomVA, shadowVA;

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
static float diffuseLight[] = {0.5, 0.5, 0.5};
static float lightPosition[] = {0, 0, 8};
static float materialD[] = {0.8, 0.8, 0.8};
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

	sphere_bind(sphere, vertexPosLoc, vertexColLoc, vertexNormalLoc);

	glUniform3fv(ambientLightLoc, 1, ambientLight);
	glUniform3fv(diffuseLightLoc, 1, diffuseLight);
	glUniform3fv(lightPositionLoc, 1, lightPosition);
	glUniform3fv(materialALoc, 1, materialA);
	glUniform3fv(materialDLoc, 1, materialD);
	glUniform3fv(materialSLoc, 1, materialS);
	glUniform1f(exponentLoc, exponent);

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CCW);
}

static void initShadow()
{
	float theta = 0;
	float dTheta = 2 * M_PI / 40;
	Vertex positions[42];
	Vertex colors[42];
	positions[0].x = 0;
	positions[0].y = 0;
	positions[0].z = 0;
	colors[0].x = 0;
	colors[0].y = 0;
	colors[0].z = 0;
	for (int i = 0; i < 42; i++)
	{
		positions[i].x = SPHERE_RADIUS * cos(theta);
		positions[i].y = 0;
		positions[i].z = SPHERE_RADIUS * sin(theta);
		colors[i].x = 0;
		colors[i].y = 0;
		colors[i].z = 0;
		theta += dTheta;
	}

	glUseProgram(programId);
	glGenVertexArrays(1, &shadowVA);
	glBindVertexArray(shadowVA);
	GLuint buffers[2];
	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexPosLoc);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);
}

static void initRoom()
{
	float w1 = -ROOM_WIDTH / 2, w2 = ROOM_WIDTH / 2;
	float h1 = -ROOM_HEIGHT / 2, h2 = ROOM_HEIGHT / 2;
	float d1 = -ROOM_DEPTH / 2, d2 = ROOM_DEPTH / 2;

	float positions[] = {
		w1, h2, d1, w1, h1, d1, w2, h1, d1, w2, h1, d1, w2, h2, d1, w1, h2, d1, // Frente
		w2, h2, d2, w2, h1, d2, w1, h1, d2, w1, h1, d2, w1, h2, d2, w2, h2, d2, // Atr�s
		w1, h2, d2, w1, h1, d2, w1, h1, d1, w1, h1, d1, w1, h2, d1, w1, h2, d2, // Izquierda
		w2, h2, d1, w2, h1, d1, w2, h1, d2, w2, h1, d2, w2, h2, d2, w2, h2, d1, // Derecha
		w1, h1, d1, w1, h1, d2, w2, h1, d2, w2, h1, d2, w2, h1, d1, w1, h1, d1, // Abajo
		w1, h2, d2, w1, h2, d1, w2, h2, d1, w2, h2, d1, w2, h2, d2, w1, h2, d2	// Arriba
	};

	float colors[] = {
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, // Frente
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, // Atr�s
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, // Izquierda
		1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, // Derecha
		0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, // Abajo
		1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, // Arriba
	};

	float normals[] = {
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,		// Frente
		0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, // Atr�s
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,		// Izquierda
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, // Derecha
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,		// Abajo
		0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, // Arriba
	};

	glUseProgram(programId);
	glGenVertexArrays(1, &roomVA);
	glBindVertexArray(roomVA);
	GLuint buffers[3];
	glGenBuffers(3, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexPosLoc);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexNormalLoc);
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

	rotateY(&viewMatrix, -cameraAngle);
	translate(&viewMatrix, -cameraX, 0, -cameraZ);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix.values);
	glUniform3f(cameraLoc, cameraX, 0, cameraZ);

	mIdentity(&modelMatrix);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	glBindVertexArray(roomVA);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	translate(&modelMatrix, spherePosition.x, -ROOM_HEIGHT / 2 + 0.01, spherePosition.z);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	glBindVertexArray(shadowVA);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 41);

	static float angle = -45;

	mIdentity(&modelMatrix);
	translate(&modelMatrix, spherePosition.x, spherePosition.y, spherePosition.z);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	sphere_draw(sphere);

	angle += 1;
	if (angle >= 360.0)
		angle -= 360.0;

	spherePosition.x += sphereVelocity.x;
	spherePosition.y += sphereVelocity.y;
	spherePosition.z += sphereVelocity.z;

	if (spherePosition.x + SPHERE_RADIUS >= ROOM_WIDTH / 2)
	{
		sphereVelocity.x *= -1;
	}
	else if (spherePosition.x - SPHERE_RADIUS <= -ROOM_WIDTH / 2)
	{
		sphereVelocity.x *= -1;
	}

	if (spherePosition.y + SPHERE_RADIUS >= ROOM_HEIGHT / 2)
	{
		sphereVelocity.y *= -1;
	}
	else if (spherePosition.y - SPHERE_RADIUS <= -ROOM_HEIGHT / 2)
	{
		sphereVelocity.y *= -1;
	}

	if (spherePosition.z + SPHERE_RADIUS >= ROOM_DEPTH / 2)
	{
		sphereVelocity.z *= -1;
	}
	else if (spherePosition.z - SPHERE_RADIUS <= -ROOM_DEPTH / 2)
	{
		sphereVelocity.z *= -1;
	}

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

	glutCreateWindow("Sphere");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutReshapeFunc(reshapeFunc);
	glewInit();
	Vertex sphereColor1 = {0.8, 0.3, 0.8};
	sphere = sphere_create(SPHERE_RADIUS, 40, 40, sphereColor1);
	initShaders();
	initRoom();
	initShadow();

	glClearColor(0.1, 0.1, 0.1, 1.0);
	glutMainLoop();
	return 0;
}
