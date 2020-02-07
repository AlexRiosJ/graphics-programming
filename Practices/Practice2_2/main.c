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

void init() {
	GLuint fshId = compileShader("./Shaders/color.fsh", GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
	glAttachShader(programId, fshId);
	glLinkProgram(programId);

	float vertexPos[] = { -1, -1, -1, 1, 1, -1, 1, 1 };
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(2);
	glUseProgram(programId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	int w = 600, h = 400;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutCreateWindow("Practice 2.2");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);

	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
