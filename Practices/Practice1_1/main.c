#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

static float trianglePosAndColor[] = {
		-0.9, -0.9,
		-0.9,  0.9,
		 0.0, -0.8,
		 0.0,  0.8,
		 0.9, -0.9,
		 0.9,  0.9,
		 1, 1, 0,
		 1, 1, 0,
		 1, 0, 1,
		 1, 0, 1,
		 0, 1, 1,
		 0, 1, 1
};

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

void specialKeyPressed(int key, int x, int y) {
	if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) {
		exit(0);
	}
}

static void initTriangle() {
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosAndColor), trianglePosAndColor, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosAndColor), trianglePosAndColor + 2 * 6, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_COLOR_ARRAY);
}

static void draw() { // drawTriangle
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
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
	glutCreateWindow("My first GLUT window");

	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutDisplayFunc(draw);

	glewInit();

	initTriangle();

	glClearColor(0.2, 0.2, 0.2, 1.0);

	glutMainLoop();

	return 0;
}
