#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "utils.h"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

GLuint programId;
GLuint vertexArrayId;
GLuint bufferId[2];
GLuint colorLoc;
GLuint colorSwapLoc;

float color1R = 0.5, color1G = 0.2, color1B = 0.4;
float color2R = 0.9, color2G = 0.8, color2B = 0.3;

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	} else if (key == 13) {
		GLuint temp = colorLoc;
		colorLoc = colorSwapLoc;
		colorSwapLoc = temp;
	}
}

void genereteHexagonPoints(float r, float array[]) {
	for(int i = 0; i < 6 * 2; i += 2) {
		array[i] = r * cos(((i - 2) / 2) * M_PI / 3);
		array[i + 1] = r * sin(((i - 2) / 2) * M_PI / 3);
	}
}

void init() {
	GLuint fshId = compileShader("./shaders/color.fsh", GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
	glAttachShader(programId, fshId);
	glLinkProgram(programId);

	glGenVertexArrays(2, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glGenBuffers(2, bufferId);

	float innerHex[6 * 2];
	float outerHex[6 * 2];

	genereteHexagonPoints(0.5, innerHex);
	genereteHexagonPoints(0.8, outerHex);
	
	glBindVertexArray(bufferId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(innerHex), innerHex, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindVertexArray(bufferId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(outerHex), outerHex, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	colorLoc = glGetUniformLocation(programId, "color");
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(bufferId[1]);
	glUniform4f(colorLoc, color1R, color1G, color1B, 1.0);
	glUniform4f(colorSwapLoc, color2R, color2G, color2B, 1.0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glBindVertexArray(bufferId[0]);
	glUniform4f(colorSwapLoc, color1R, color1G, color1B, 1.0);
	glUniform4f(colorLoc, color2R, color2G, color2B, 1.0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glutSwapBuffers();
}

static void timerFunc(int id) {
	glutTimerFunc(5, timerFunc, 1);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutCreateWindow("Practice 3.1");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(draw);
	glutTimerFunc(5, timerFunc, 1);
	glewInit();
	
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}
