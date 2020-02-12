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
GLuint color1Loc;
GLuint color2Loc;

float color1R = 0.5, color1G = 0.2, color1B = 0.4;
float color2R = 0.9, color2G = 0.8, color2B = 0.3;
int color1 = 1;

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	} else if (key == 13) {
		GLuint temp = color1Loc;
		color1Loc = color2Loc;
		color2Loc = temp;
		glUniform4f(color1Loc, color1R, color1G, color1B, 1.0);
		glUniform4f(color2Loc, color2R, color2G, color2B, 1.0);
	}
}

void genereteHexagonPoints(float r, float array[], float z) {
	for(int i = 0; i < 6 * 3; i += 3) {
		array[i] = r * cos(((i - 3) / 3) * M_PI / 3);
		array[i + 1] = r * sin(((i - 3) / 3) * M_PI / 3);
		array[i + 2] = z;
		// printf("%.2f, %.2f, %.2f\n", array[i], array[i + 1], array[i + 2]);
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

	float innerHex[6 * 3];
	float outerHex[6 * 3];

	genereteHexagonPoints(0.5, innerHex, 0);
	genereteHexagonPoints(0.8, outerHex, -0.5);
	
	glBindVertexArray(bufferId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(innerHex), innerHex, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindVertexArray(bufferId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(outerHex), outerHex, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	color1Loc = glGetUniformLocation(programId, "color1");
	color2Loc = glGetUniformLocation(programId, "color2");
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(bufferId[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glBindVertexArray(bufferId[1]);
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
