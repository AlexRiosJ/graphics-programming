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
	GLuint vshId = compileShader("./Shaders/position.vsh", GL_VERTEX_SHADER);
	GLuint fshId = compileShader("./Shaders/orange.fsh", GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
	glAttachShader(programId, vshId);
	glAttachShader(programId, fshId);
	glLinkProgram(programId);

	float trianglePos[] = { 0.0, 0.9, -0.9, -0.9, 0.9, -0.9 };
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePos), trianglePos, GL_STATIC_DRAW);

	GLuint vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
	glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPosLoc);

//	glVertexPointer(2, GL_FLOAT, 0, 0);
//	glEnableClientState(GL_VERTEX_ARRAY);
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
	glutCreateWindow("Working With Shaders");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);

	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
