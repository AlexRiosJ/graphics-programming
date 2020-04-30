#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "transforms.h"
#include "utils.h"

static GLuint programIdQ, programIdT, patchPositionLoc, patchColorLoc, vaId;
static Mat4 projectionMatrix, modelMatrix, viewMatrix;
static float cameraZ = 0;
static int tessLevelInner = 2, tessLevelOuter = 2;

static void initPatches()
{
	GLuint bufferId[2];
	glGenVertexArrays(1, &vaId);
	glBindVertexArray(vaId);
	glGenBuffers(2, bufferId);
	float patchPositions[] = {
		-5, -2, -10, -1, -2, -10, -1, 1, -10, -5, 1, -10, // Rect�ngulo izquierdo
		1, -3, -10, 4, -3, -10, 3, 3, -10, 1, 3, -10,	  // Rect�ngulo derecho
		0, 4, -10, -3, -4, -10, 3, -4, -10				  // Tri�ngulo
	};
	float patchColors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
						   0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
						   0.5, 0.5, 1, 0.5, 1, 0.5, 1, 0.5, 0.5};

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(patchPositions), patchPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(patchPositionLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(patchPositionLoc);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(patchColors), patchColors, GL_STATIC_DRAW);
	glVertexAttribPointer(patchColorLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(patchColorLoc);
}

static void loadShaders()
{
	GLuint vShader = compileShader("shaders/tess.vsh", GL_VERTEX_SHADER);
	if (!shaderCompiled(vShader))
		return;
	GLuint fShader = compileShader("shaders/tess.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(fShader))
		return;
	GLuint tcShaderQ = compileShader("shaders/tessQ.csh", GL_TESS_CONTROL_SHADER);
	if (!shaderCompiled(tcShaderQ))
		return;
	GLuint teShaderQ = compileShader("shaders/tessQ.esh", GL_TESS_EVALUATION_SHADER);
	if (!shaderCompiled(teShaderQ))
		return;

	programIdQ = glCreateProgram();
	glAttachShader(programIdQ, vShader);
	glAttachShader(programIdQ, tcShaderQ);
	glAttachShader(programIdQ, teShaderQ);
	glAttachShader(programIdQ, fShader);
	glLinkProgram(programIdQ);

	GLuint tcShaderT = compileShader("shaders/tessT.csh", GL_TESS_CONTROL_SHADER);
	if (!shaderCompiled(tcShaderT))
		return;
	GLuint teShaderT = compileShader("shaders/tessT.esh", GL_TESS_EVALUATION_SHADER);
	if (!shaderCompiled(teShaderT))
		return;

	programIdT = glCreateProgram();
	glAttachShader(programIdT, vShader);
	glAttachShader(programIdT, tcShaderT);
	glAttachShader(programIdT, teShaderT);
	glAttachShader(programIdT, fShader);
	glLinkProgram(programIdT);

	glUseProgram(programIdT);
	patchPositionLoc = glGetAttribLocation(programIdQ, "patchPosition");
	patchColorLoc = glGetAttribLocation(programIdQ, "patchColor");

	//	float outer_levels[] = { 3, 5, 2, 4 };
	//	float inner_levels[] = { 7, 6 };
	//	glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outer_levels);
	//	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, inner_levels);
}

static void reshapeFunc(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	float aspect = (float)w / h;
	setPerspective(&projectionMatrix, 53, aspect, -0.1, -500);
}

static void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	mIdentity(&viewMatrix);
	translate(&viewMatrix, 0, 0, -cameraZ);

	static float angle = 0;
	mIdentity(&modelMatrix);
	rotateZ(&modelMatrix, angle += 0.2);
	glBindVertexArray(vaId);

	glUseProgram(programIdQ);
	glUniformMatrix4fv(glGetUniformLocation(programIdQ, "projectionMatrix"), 1, true, projectionMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(programIdQ, "viewMatrix"), 1, true, viewMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(programIdQ, "modelMatrix"), 1, true, modelMatrix.values);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawArrays(GL_PATCHES, 0, 8);

	glUseProgram(programIdT);
	glUniformMatrix4fv(glGetUniformLocation(programIdT, "projectionMatrix"), 1, true, projectionMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(programIdT, "viewMatrix"), 1, true, viewMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(programIdT, "modelMatrix"), 1, true, modelMatrix.values);
	glUniform1f(glGetUniformLocation(programIdT, "cameraZ"), cameraZ);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_PATCHES, 8, 3);

	glutSwapBuffers();
}

static void keyPressedFunc(unsigned char key, int x, int y)
{
	if (glutGetModifiers() == GLUT_ACTIVE_ALT)
	{
		if (key == '-')
		{
			if (tessLevelOuter == 1)
				return;
			tessLevelOuter--;
			glUseProgram(programIdQ);
			glUniform1f(glGetUniformLocation(programIdQ, "tessLevelOuter"), tessLevelOuter);
			glUseProgram(programIdT);
			glUniform1f(glGetUniformLocation(programIdQ, "tessLevelOuter"), tessLevelOuter);
			glutPostRedisplay();
		}
		else if (key == '+')
		{
			tessLevelOuter++;
			glUseProgram(programIdQ);
			glUniform1f(glGetUniformLocation(programIdQ, "tessLevelOuter"), tessLevelOuter);
			glUseProgram(programIdT);
			glUniform1f(glGetUniformLocation(programIdQ, "tessLevelOuter"), tessLevelOuter);
			glutPostRedisplay();
		}
		return;
	}
	if (key == 'r' || key == 'R')
	{
		cameraZ++;
	}
	else if (key == 'f' || key == 'F')
	{
		cameraZ--;
	}
	else if (key == '-')
	{
		if (tessLevelInner == 1)
			return;
		tessLevelInner--;
		glUseProgram(programIdQ);
		glUniform1f(glGetUniformLocation(programIdQ, "tessLevelInner"), tessLevelInner);
		glUseProgram(programIdT);
		glUniform1f(glGetUniformLocation(programIdQ, "tessLevelInner"), tessLevelInner);
		glutPostRedisplay();
	}
	else if (key == '+')
	{
		tessLevelInner++;
		glUseProgram(programIdQ);
		glUniform1f(glGetUniformLocation(programIdQ, "tessLevelInner"), tessLevelInner);
		glUseProgram(programIdT);
		glUniform1f(glGetUniformLocation(programIdQ, "tessLevelInner"), tessLevelInner);
		glutPostRedisplay();
	}
	else if (key == 27)
		exit(0);
}

static void timerFunc(int id)
{
	glutTimerFunc(10, timerFunc, id);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(750, 600);
	glutCreateWindow("Test Tessellation Shader");
	glutDisplayFunc(displayFunc);
	glutTimerFunc(10, timerFunc, 1);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyPressedFunc);
	glewInit();
	printf("%s\n", glGetString(GL_VERSION));
	loadShaders();
	initPatches();
	glClearColor(0.05, 0.05, 0.05, 1);
	glLineWidth(2);
	glPointSize(5);
	glutMainLoop();
	return 0;
}
