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
#define toRadians(deg) deg *M_PI / 180.0

Sphere spheres[3];
Vertex spherePosition[3];
Vertex sphereVelocity[3];
static const float SPHERE_RADIUS = 0.5;

Cylinder cylinder;

Vertex cameraPosition = {0, 0, 0};
float cameraSpeed = 0.05;

unsigned char keys[256];

static const int ROOM_WIDTH = 15;
static const int ROOM_HEIGHT = 10;
static const int ROOM_DEPTH = 20;

static float movex = 0, movey = 0;

GLuint roomVA, shadowVA;

static Mat4 projectionMatrix, modelMatrix, viewMatrix;
static GLuint programId, vertexPosLoc, vertexColLoc, vertexNormalLoc, modelMatrixLoc, viewMatrixLoc, projMatrixLoc;
static GLuint ambientLightLoc, diffuseLightLoc, lightPositionLoc, materialALoc, materialDLoc, materialSLoc, exponentLoc, cameraLoc;

static float ambientLight[] = {1, 1, 1};
static float materialA[] = {0.5, 0.5, 0.5};
static float diffuseLight[] = {0.5, 0.5, 0.5};
static float lightPosition[] = {0, 0, 0};
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
		1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1  // Arriba
	};

	float normals[] = {
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,		// Frente
		0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, // Atr�s
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,		// Izquierda
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, // Derecha
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,		// Abajo
		0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0	// Arriba
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

static void move()
{
	float threshold = 0.2;
	float w1 = -ROOM_WIDTH / 2 + threshold, w2 = ROOM_WIDTH / 2 - threshold;
	float h1 = -ROOM_HEIGHT / 2 + threshold, h2 = ROOM_HEIGHT / 2 - threshold;
	float d1 = -ROOM_DEPTH / 2 + threshold, d2 = ROOM_DEPTH / 2 - threshold;

	float nextForwardXPosition = cameraSpeed * -sin(toRadians(movex * 0.08));
	float nextForwardYPosition = cameraSpeed * sin(toRadians(movey * 0.08));
	float nextForwardZPosition = cameraSpeed * cos(toRadians(movex * 0.08));

	float nextSideXPosition = cameraSpeed * -cos(toRadians(movex * 0.08));
	float nextSideZPosition = cameraSpeed * -sin(toRadians(movex * 0.08));

	if (keys['w'])
	{
		if (cameraPosition.x - nextForwardXPosition > w1 && cameraPosition.x - nextForwardXPosition < w2)
			cameraPosition.x -= nextForwardXPosition;
		if (cameraPosition.y - nextForwardYPosition > h1 && cameraPosition.y - nextForwardYPosition < h2)
			cameraPosition.y -= nextForwardYPosition;
		if (cameraPosition.z - nextForwardZPosition > d1 &&
			cameraPosition.z - nextForwardZPosition < d2)
			cameraPosition.z -= nextForwardZPosition;
	}
	if (keys['s'])
	{
		if (cameraPosition.x + nextForwardXPosition > w1 && cameraPosition.x + nextForwardXPosition < w2)
			cameraPosition.x += nextForwardXPosition;
		if (cameraPosition.y + nextForwardYPosition > h1 && cameraPosition.y + nextForwardYPosition < h2)
			cameraPosition.y += nextForwardYPosition;
		if (cameraPosition.z + nextForwardZPosition > d1 &&
			cameraPosition.z + nextForwardZPosition < d2)
			cameraPosition.z += nextForwardZPosition;
	}
	if (keys['a'])
	{
		if (cameraPosition.x + nextSideXPosition > w1 && cameraPosition.x + nextSideXPosition < w2)
			cameraPosition.x += nextSideXPosition;
		if (cameraPosition.z + nextSideZPosition > d1 && cameraPosition.z + nextSideZPosition < d2)
			cameraPosition.z += nextSideZPosition;
	}
	if (keys['d'])
	{
		if (cameraPosition.x - nextSideXPosition > w1 && cameraPosition.x - nextSideXPosition < w2)
			cameraPosition.x -= nextSideXPosition;
		if (cameraPosition.z - nextSideZPosition > d1 && cameraPosition.z - nextSideZPosition < d2)
			cameraPosition.z -= nextSideZPosition;
	}
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);

	mIdentity(&viewMatrix);

	move();
	glUniform3f(cameraLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	rotateX(&viewMatrix, movey * 0.08);
	rotateY(&viewMatrix, movex * 0.08);
	translate(&viewMatrix, -cameraPosition.x, -cameraPosition.y, -cameraPosition.z);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix.values);

	mIdentity(&modelMatrix);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	glBindVertexArray(roomVA);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	static float angle = -45;

	mIdentity(&modelMatrix);
	translate(&modelMatrix, 0, 0, -5);
	rotateX(&modelMatrix, angle);
	rotateZ(&modelMatrix, angle);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
	cylinder_draw(cylinder);

	angle += 1;
	if (angle >= 360.0)
		angle -= 360.0;

	for (int i = 0; i < 3; i++)
	{
		mIdentity(&modelMatrix);
		translate(&modelMatrix, spherePosition[i].x, -ROOM_HEIGHT / 2 + 0.01, spherePosition[i].z);
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
		glBindVertexArray(shadowVA);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 41);

		mIdentity(&modelMatrix);
		translate(&modelMatrix, spherePosition[i].x, spherePosition[i].y, spherePosition[i].z);
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix.values);
		sphere_draw(spheres[i]);

		if (keys[13]) // Movimiento pausado por defecto (pulsar ENTER para reanudar)
		{
			spherePosition[i].x += sphereVelocity[i].x;
			spherePosition[i].y += sphereVelocity[i].y;
			spherePosition[i].z += sphereVelocity[i].z;

			if (spherePosition[i].x + SPHERE_RADIUS >= ROOM_WIDTH / 2 || spherePosition[i].x - SPHERE_RADIUS <= -ROOM_WIDTH / 2)
			{
				sphereVelocity[i].x *= -1;
			}

			if (spherePosition[i].y + SPHERE_RADIUS >= ROOM_HEIGHT / 2 || spherePosition[i].y - SPHERE_RADIUS <= -ROOM_HEIGHT / 2)
			{
				sphereVelocity[i].y *= -1;
			}

			if (spherePosition[i].z + SPHERE_RADIUS >= ROOM_DEPTH / 2 || spherePosition[i].z - SPHERE_RADIUS <= -ROOM_DEPTH / 2)
			{
				sphereVelocity[i].z *= -1;
			}
		}
	}

	glutSwapBuffers();
}

static void timerFunc(int id)
{
	glutWarpPointer(glutGet(GLUT_WINDOW_X) + glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_Y) + glutGet(GLUT_WINDOW_HEIGHT) / 2);
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

static void keyPressed(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 13)					 // Enter
		keys[13] = keys[13] ? 0 : 1; // Cambiar entre 1 y 0 cada vez que se pulsa
	else
		keys[key] = 1;
}

static void keyReleased(unsigned char key, int x, int y)
{
	if (key != 13)
		keys[key] = 0;
}

void rotateCamera(int x, int y)
{
	movex += (float)(x - glutGet(GLUT_WINDOW_WIDTH) / 2);
	movey += (float)(y - glutGet(GLUT_WINDOW_HEIGHT) / 2);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

static void mouseMove(int x, int y)
{
	rotateCamera(x, y);
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Sphere");
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutReshapeFunc(reshapeFunc);
	glewInit();
	initShaders();

	for (int i = 0; i < 3; i++)
	{
		Vertex sphereColor = {i % 3, (i + 1) % 3, (i + 2) % 3};
		spheres[i] = sphere_create(SPHERE_RADIUS, 40, 40, sphereColor);
		sphere_bind(spheres[i], vertexPosLoc, vertexColLoc, vertexNormalLoc);
		spherePosition[i].x = 0;
		spherePosition[i].y = 0;
		spherePosition[i].z = 0;
		sphereVelocity[i].x = (i + 1) * 0.03;
		sphereVelocity[i].y = (i + 1) * 0.03;
		sphereVelocity[i].z = (i + 1) * 0.03;
	}

	Vertex cylinderBottomColor = {1, 0, 0};
	Vertex cylinderTopColor = {0, 0, 1};
	cylinder = cylinder_create(2, 0.2, 0.4, 50, 50, cylinderBottomColor, cylinderTopColor);
	cylinder_bind(cylinder, vertexPosLoc, vertexColLoc, vertexNormalLoc);

	initRoom();
	initShadow();

	glClearColor(0, 0, 0, 1.0);
	glutMainLoop();
	return 0;
}
