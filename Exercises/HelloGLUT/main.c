#include <stdio.h>
#include <GL/freeglut.h>

void keyPressed(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
	printf("%c", key);
}

void specialKeyPressed(int key, int x, int y) {
	if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT) {
		exit(0);
	}
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

	glutMainLoop();
//	while(1) {
//		glutMainLoopEvent();
//	}
	return 0;
}
