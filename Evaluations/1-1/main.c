#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"

const int color = 1;
float red, green, blue;
const float MAXPOS = 1.0;

GLuint vertexArrayId;
GLuint programID;
GLuint heightLoc, widthLoc;


void initShaders()
{
    GLuint vshId = compileShader("shaders/colorPosition.vsh", GL_VERTEX_SHADER);

    GLuint fshId;
    if(color) 
    {
        fshId = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
    }
    else
    {
        fshId = compileShader("shaders/grey.fsh", GL_FRAGMENT_SHADER);
    }

    programID = glCreateProgram();
    glAttachShader(programID, vshId);
    glAttachShader(programID, fshId);
    glLinkProgram(programID);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    //Creamos buffers para mandarlos a los shaders
    GLuint bufferId[2];
    glGenBuffers(2, bufferId);

    //Mandamos el de posicion
    float pos[] = {
        -MAXPOS, MAXPOS,
        MAXPOS, MAXPOS,
        -MAXPOS, -MAXPOS,
        MAXPOS, -MAXPOS
    };

    glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    GLuint vertexPosLoc = glGetAttribLocation(programID, "vertexPosition");
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPosLoc);

    heightLoc = glGetUniformLocation(programID, "height");
    widthLoc = glGetUniformLocation(programID, "width");
}

void setWidthAndHeight()
{
    int height, width;

    height = glutGet(GLUT_WINDOW_HEIGHT);
    width =  glutGet(GLUT_WINDOW_WIDTH);

    glUniform1d(heightLoc, height);
    glUniform1d(widthLoc, width);
}

static void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertexArrayId);
    glUseProgram(programID);

    int height, width;

    height = glutGet(GLUT_WINDOW_HEIGHT);
    width = glutGet(GLUT_WINDOW_WIDTH);

    glUniform1i(heightLoc, height);
    glUniform1i(widthLoc, width);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    // 1) Configurar ventana
    glutInit(&argc, argv);

    int windowWidth = 600, windowHeight = 600;
    int desktopWidth = glutGet(GLUT_SCREEN_WIDTH);
    int desktopHeight = glutGet(GLUT_SCREEN_HEIGHT);

    int windowInitX = (desktopWidth - windowWidth) / 2;
    int windowInitY = (desktopHeight - windowHeight) / 2;

    glutInitWindowPosition(windowInitX, windowInitY);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Julia");

    glutDisplayFunc(draw);
    glewInit();

    initShaders();

    // 6) Background
    red = 0;
    green = 0;
    blue = 0;
    glClearColor(red, green, blue, 1.0);
    glutMainLoop();

    return 0;
}
