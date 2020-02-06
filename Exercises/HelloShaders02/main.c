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

void init() {
	GLuint vshId = compileShader("./Shaders/colorPosition.vsh", GL_VERTEX_SHADER);
	GLuint fshId = compileShader("./Shaders/color.fsh", GL_FRAGMENT_SHADER);
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
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(1);
	glUseProgram(programId);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	int w = 600, h = 400;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutCreateWindow("Working With Shaders 2");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);

	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
