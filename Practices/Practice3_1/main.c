#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "utils.h"


void keyPressed(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

void genereteHexagonPoints(float r, float array[], float z) {
	for(int i = 0; i < 6 * 3; i += 3) {
		array[i] = r * cos(((i - 3) / 3) * M_PI / 3);
		array[i + 1] = r * sin(((i - 3) / 3) * M_PI / 3);
		array[i + 2] = z;
	}
}

GLuint programId;
GLuint vertexArrayId;
GLuint bufferId[2];

void init() {
	// GLuint fshId = compileShader("./shaders/color.fsh", GL_FRAGMENT_SHADER);
	// programId = glCreateProgram();
	// glAttachShader(programId, fshId);
	// glLinkProgram(programId);

	glGenVertexArrays(2, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glGenBuffers(2, bufferId);

	float innerHex[6 * 3];
	float outerHex[6 * 3];
	// array[0] = array[1] = array[2] = 0.0;
	genereteHexagonPoints(0.5, innerHex, 0);
	genereteHexagonPoints(0.8, outerHex, 0);
	
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(innerHex), innerHex, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindVertexArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(outerHex), outerHex, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(bufferId[0]);
	// glUseProgram(programId);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	// glUseProgram(programId);
	glBindVertexArray(bufferId[1]);
	glDrawArrays(GL_LINE_STRIP, 0, 6);

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	int w = 600, h = 600;
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
