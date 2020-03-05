#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"

static GLuint programId, va[1], vertexPosLoc, vertexColLoc, csMatrixLoc, projMatrixLoc;
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
	csMatrixLoc = glGetUniformLocation(programId, "csMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projMatrix");

	glUseProgram(programId);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

static void createSquare()
{
	float squarePos[] = {
		-2, -1, 3,
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
		-2, 1, 3};

	float squareCol[] = {
		0, 1, 0,
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
		1, 0, 1};

	GLushort squareIndex[] = {0, 1, 2, 3, 0xFFFF,
							  4, 5, 6, 7, 0xFFFF,
							  8, 9, 10, 11, 0xFFFF,
							  12, 13, 14, 15};

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

static float angle = 0;

static void display()
{
	Mat4 csMat;
	mIdentity(&csMat);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	translate(&csMat, 0, 0, -15);
	// rotateY(&csMat, angle++);
	rotateX(&csMat, angle++);
	rotateZ(&csMat, angle++);
	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMat.values);
	glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
}

static void exitFunc(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		glDeleteVertexArrays(1, va);
		exit(0);
	}
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
	// if (aspect >= 1.0)
	// {
	// 	setOrtho(&projMatrix, -10 * aspect, 10 * aspect, -10, 10, -10, 10);
	// }
	// else
	// {
	// 	setOrtho(&projMatrix, -10, 10, -10 / aspect, 10 / aspect, -10, 10);
	// }

	setPerspective(&projMatrix, 45, aspect, -6, 6);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMatrix.values);
}

int main(int argc, char **argv)
{
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
	createSquare();
	glClearColor(0.05, 0.05, 0.10, 1.0);
	glutMainLoop();
	return 0;
}
