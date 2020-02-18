#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "transforms.h"

GLuint vertexArrayId;
GLuint bufferId[3];
GLuint programId;
GLuint vertexPosLoc, vertexColLoc;

Vertex pos[] = {{-0.2, 0.2, 0.0}, {-0.2, -0.2, 0.0}, {0.2, -0.2, 0.0}, {0.2, 0.2, 0.0}};
float color[] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0};
GLushort indexes[] = {
    0, 1, 2,
    0, 2, 3};

static void initShaders()
{
    GLuint vShader = compileShader("./shaders/colorPosition.vsh", GL_VERTEX_SHADER);
    if (!shaderCompiled(vShader))
        return;

    GLuint fShader = compileShader("./shaders/color.fsh", GL_FRAGMENT_SHADER);
    if (!shaderCompiled(fShader))
        return;

    programId = glCreateProgram();
    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);
    glLinkProgram(programId);
    vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    vertexColLoc = glGetAttribLocation(programId, "vertexColor");
}

static void init()
{
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glGenBuffers(3, bufferId);

    for (int i = 0; i < 4; i++)
    {
        rotateZVertex(&pos[i], 30);
        translateVertex(&pos[i], 0.3, 0.5, 0);
    }

    processArrayBuffer(bufferId[0], pos, sizeof pos, vertexPosLoc, 3, GL_FLOAT);
    processArrayBuffer(bufferId[1], color, sizeof color, vertexColLoc, 3, GL_FLOAT);
    processIndexBuffer(bufferId[2], indexes, sizeof indexes, 0xFF);
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
    glDrawElements(GL_LINE_STRIP, 7, GL_UNSIGNED_SHORT, 0);
    glutSwapBuffers();
}

static void exitFunc(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        glDeleteVertexArrays(1, &vertexArrayId);
        exit(0);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    int w = 600, h = 600;
    int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
    int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
    glutInitWindowSize(w, h);
    glutInitWindowPosition(x, y);
    glutCreateWindow("Transforms");
    glutDisplayFunc(display);
    glutKeyboardFunc(exitFunc);
    glewInit();
    initShaders();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    init();

    glutMainLoop();
    return 0;
}