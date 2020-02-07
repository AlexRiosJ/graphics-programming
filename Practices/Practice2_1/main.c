#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define TOTAL_POINTS 200

static float points[TOTAL_POINTS * 2];
static float colors[TOTAL_POINTS * 3];

void initPoints() {
	float x = -M_PI;
	for (int i = 0, j = 0; i < TOTAL_POINTS * 2; i += 2, j += 3, x += 2 * M_PI / TOTAL_POINTS) {
		if (i & 2) {
			points[i] = x / M_PI;
			points[i + 1] = 0.8 * sin(x) + 0.2;
			colors[j] = rand() / (float) RAND_MAX;
			colors[j + 1] = rand() / (float) RAND_MAX;
			colors[j + 2] = rand() / (float) RAND_MAX;
		} else {
			points[i] = x / M_PI;
			points[i + 1] = 0.8 * sin(x) - 0.2;
			colors[j] = colors[j + 1] = colors[j + 2] = 0.2;
		}
	}
}

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

void specialKeyPressed(int key, int x, int y) {
	if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) {
		exit(0);
	}
}

static void initPlot() {
	initPoints();
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_COLOR_ARRAY);
}

static void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, TOTAL_POINTS);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	int ww = 600, wh = 400;
	glutInitWindowSize(ww, wh);

	int dw = glutGet(GLUT_SCREEN_WIDTH);
	int dh = glutGet(GLUT_SCREEN_HEIGHT);
	int wx = (dw - ww) / 2;
	int wy = (dh - wh) / 2;
	glutInitWindowPosition(wx, wy);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Practice 1.2");

	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutDisplayFunc(draw);

	glewInit();

	initPlot();

	glClearColor(0.2, 0.2, 0.2, 1.0);

	glutMainLoop();

	return 0;
}
