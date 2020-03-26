#include <GL/glew.h>
#include <GL/freeglut.h>
#include "transforms.h"
#include "utils.h"
#include <stdio.h>

static Mat4 modelMatrix, projectionMatrix, viewMatrix;
static GLuint programId, vertexPositionLoc, vertexNormalLoc, modelMatrixLoc, projectionMatrixLoc, viewMatrixLoc;
static GLuint ambientLightLoc, diffuseLightLoc, lightPositionLoc, materialALoc, materialDLoc, materialSLoc, exponentLoc, cameraLoc;

static int motionType = 0;
static float observerX = 0, observerZ = 0;
static float cubeAngle = 0;
static GLuint cubeVA;
static int N = 15;

static float ambientLight[] = {0.5, 0.5, 0.5};
static float materialA[] = {0.0, 0.0, 1.0};

static float diffuseLight[] = {1.0, 1.0, 1.0};
static float lightPosition[] = {0.0, 0.0, -1.0};
static float materialD[] = {0.6, 0.6, 0.6};
static float materialS[] = {0.7, 0.7, 0.7};
static float exponent = 64;

static void initShaders()
{
	GLuint vShader = compileShader("shaders/gouraud.vsh", GL_VERTEX_SHADER);
	if (!shaderCompiled(vShader))
		return;

	GLuint fShader = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(fShader))
		return;

	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);

	vertexPositionLoc = glGetAttribLocation(programId, "vertexPosition");
	vertexNormalLoc = glGetAttribLocation(programId, "vertexNormal");
	modelMatrixLoc = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
	projectionMatrixLoc = glGetUniformLocation(programId, "projMatrix");
	//	Obtenci�n de los id de las variables uniformes relacionadas con la iluminaci�n
	ambientLightLoc = glGetUniformLocation(programId, "ambientLight");
	diffuseLightLoc = glGetUniformLocation(programId, "diffuseLight");
	lightPositionLoc = glGetUniformLocation(programId, "lightPosition");
	materialALoc = glGetUniformLocation(programId, "materialA");
	materialDLoc = glGetUniformLocation(programId, "materialD");
	materialSLoc = glGetUniformLocation(programId, "materialS");
	exponentLoc = glGetUniformLocation(programId, "exponent");
	cameraLoc = glGetUniformLocation(programId, "camera");

	glUseProgram(programId);
	glUniform3fv(ambientLightLoc, 1, ambientLight);
	glUniform3fv(diffuseLightLoc, 1, diffuseLight);
	glUniform3fv(lightPositionLoc, 1, lightPosition);
	glUniform3fv(materialALoc, 1, materialA);
	glUniform3fv(materialDLoc, 1, materialD);
	glUniform3fv(materialSLoc, 1, materialS);
	glUniform1f(exponentLoc, exponent);
}

static void initCube()
{
	float l1 = -0.5, l2 = 0.5;
	float positions[] = {
		l1, l1, l2, l2, l1, l2, l1, l2, l2, l2, l1, l2, l2, l2, l2, l1, l2, l2, // Frente
		l2, l1, l1, l1, l1, l1, l2, l2, l1, l1, l1, l1, l1, l2, l1, l2, l2, l1, // Atr�s
		l1, l1, l1, l1, l1, l2, l1, l2, l1, l1, l1, l2, l1, l2, l2, l1, l2, l1, // Izquierda
		l2, l2, l1, l2, l2, l2, l2, l1, l1, l2, l2, l2, l2, l1, l2, l2, l1, l1, // Derecha
		l1, l1, l1, l2, l1, l1, l1, l1, l2, l2, l1, l1, l2, l1, l2, l1, l1, l2, // Abajo
		l2, l2, l1, l1, l2, l1, l2, l2, l2, l1, l2, l1, l1, l2, l2, l2, l2, l2  // Arriba
	};

	float normals[] = {
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,		// Frente
		0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, // Atr�s
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, // Izquierda
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,		// Derecha
		0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, // Abajo
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,		// Arriba
	};

	glGenVertexArrays(1, &cubeVA);
	glBindVertexArray(cubeVA);
	GLuint buffers[2];
	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexPositionLoc);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexNormalLoc);

	//	Enviar los datos de la luz ambiente, fuentes de luz y propiedades materiales
}

static void moveForward()
{
	observerZ -= (N / 40.0);
}

static void moveBack()
{
	observerZ += (N / 40.0);
}

static void turnLeft()
{
	observerX -= (N / 40.0);
}

static void turnRight()
{
	observerX += (N / 40.0);
}

static void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(cubeVA);

	switch (motionType)
	{
	case 1:
		moveForward();
		break;
	case 2:
		moveBack();
		break;
	case 3:
		turnRight();
		break;
	case 4:
		turnLeft();
	}

	glUniform3f(cameraLoc, observerX, 0, observerZ);

	mIdentity(&viewMatrix);
	translate(&viewMatrix, -observerX, 0, -observerZ);
	glUniformMatrix4fv(viewMatrixLoc, 1, true, viewMatrix.values);

	mIdentity(&modelMatrix);
	if (N % 2 == 0)
		translate(&modelMatrix, -N / 2 - 0.5, -N / 2 - 0.5, -2 * N);
	else
		translate(&modelMatrix, -(N + 1) / 2, -(N + 1) / 2, -2 * N);
	int i, j;
	for (i = 1; i <= N; i++)
	{
		translate(&modelMatrix, 1, 0, 0);
		pushMatrix(&modelMatrix);
		for (j = 1; j <= N; j++)
		{
			translate(&modelMatrix, 0, 1, 0);
			pushMatrix(&modelMatrix);
			rotateX(&modelMatrix, cubeAngle);
			rotateY(&modelMatrix, cubeAngle);
			scale(&modelMatrix, 0.7, 0.8, 0.4);
			glUniformMatrix4fv(modelMatrixLoc, 1, true, modelMatrix.values);
			glUniform3f(materialALoc, (float)(i + j) / N / 2, (float)i / N / 2, (float)j / N / 2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			popMatrix(&modelMatrix);
		}
		popMatrix(&modelMatrix);
	}

	cubeAngle += 0.5;

	glutSwapBuffers();
}

static void reshapeFunc(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	float aspect = (float)w / h;
	setPerspective(&projectionMatrix, 45, aspect, -0.1, -500);
	glUniformMatrix4fv(projectionMatrixLoc, 1, true, projectionMatrix.values);
}

static void timerFunc(int id)
{
	glutTimerFunc(10, timerFunc, id);
	glutPostRedisplay();
}

static void keyReleasedFunc(int key, int x, int y)
{
	motionType = 0;
}

static void specialKeyPressedFunc(int key, int x, int y)
{
	switch (key)
	{
	case 101:
		motionType = 1;
		break;
	case 103:
		motionType = 2;
		break;
	case 102:
		motionType = 3;
		break;
	case 100:
		motionType = 4;
	}
}

static void keyPressedFunc(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lighting");
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutTimerFunc(10, timerFunc, 1);
	glutKeyboardFunc(keyPressedFunc);
	glutSpecialFunc(specialKeyPressedFunc);
	glutSpecialUpFunc(keyReleasedFunc);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	initShaders();
	initCube();
	glClearColor(0.6, 0.6, 0.8, 1.0);
	glutMainLoop();
	return 0;
}
