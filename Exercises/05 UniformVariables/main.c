#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

GLuint programId;
GLuint vertexArrayId;
GLuint startXLoc;
GLuint factorLoc;
GLuint mouseLoc;

void init() {
	GLuint vshId = compileShader("./shaders/colorPosition.vsh", GL_VERTEX_SHADER);
	GLuint fshId = compileShader("./shaders/color.fsh", GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
	glAttachShader(programId, vshId);
	glAttachShader(programId, fshId);
	glLinkProgram(programId);

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	GLuint bufferId[2];

	float trianglePos[] = { 0.0, 0.9, -0.9, -0.9, 0.9, -0.9 };
	glGenBuffers(2, bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePos), trianglePos,
	GL_STATIC_DRAW);
	GLuint vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
	glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPosLoc);

	float triangleCol[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	glGenBuffers(2, bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleCol), triangleCol,
	GL_STATIC_DRAW);
	GLuint vertexColorLoc = glGetAttribLocation(programId, "vertexColor");
	glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColorLoc);

	startXLoc = glGetUniformLocation(programId, "startX");
	factorLoc = glGetUniformLocation(programId, "factor");
	mouseLoc = glGetUniformLocation(programId, "mouse");
}

float startX = 0.0;
float factorR = 1, factorG = 1, factorB = 1;

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(1);
	glUseProgram(programId);
	glUniform1f(startXLoc, startX);
	glUniform3f(factorLoc, factorR, factorG, factorB);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	startX += 0.005;
	if(startX >= 1.9){
		startX = -1.9;
	}
	factorR *= 0.99;
	factorG *= 0.99;
	factorB *= 0.99;
	if(factorR <= 0.2) factorR = 1.0;
	if(factorG <= 0.2) factorG = 1.0;
	if(factorB <= 0.2) factorB = 1.0; 
}

static void timerFunc(int id) {
	glutTimerFunc(5, timerFunc, 1);
	glutPostRedisplay();
}

static void mouseMotionFunc(int x, int y) {
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	glUniform2i(mouseLoc, x, y);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	int w = 600, h = 400;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutCreateWindow("Uniform Variables");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);

	glutTimerFunc(5, timerFunc, 1);
	glutPassiveMotionFunc(mouseMotionFunc);
	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
