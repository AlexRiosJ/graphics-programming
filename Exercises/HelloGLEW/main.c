#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

static const float max = 0.9;

static float trianglePos[] = { -max, -max, 0, max, -max, 0, 0, max, 0 };

static float triangleColor[] = { 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0 };

static GLenum primitive = GL_POINTS;

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
	if (key == 13) {
		primitive = (primitive + 1) % 8;
		glutPostRedisplay();
	}
}

void specialKeyPressed(int key, int x, int y) {
	if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) {
		exit(0);
	}
}

static void initTriangle() {
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePos), trianglePos, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_COLOR_ARRAY);
}

static void draw() { // drawTriangle
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(1);
	glDrawArrays(primitive, 0, 3);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	printf("Hello GLUT!\n");

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

	if (GLEW_VERSION_3_3) {
		printf("Soporte OpenGL 3.3\n");
	}
	printf("%s\n", glGetString(GL_VERSION));

	initTriangle();

	glClearColor(0.6, 0.7, 0.8, 1.0);

	glutMainLoop();

	return 0;
}
