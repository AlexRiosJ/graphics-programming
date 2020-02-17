#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"

static void init()
{
    GLuint programId = glCreateProgram();
    float pos[] = {-0.2, 0.2, -0.2, -0.2, 0.2, -0.2, 0.2, 0.2};
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    GLuint vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPosLoc);
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
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
	glutCreateWindow("Transforms");
	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	init();

	glutMainLoop();
	return 0;
}