#version 330

in vec3 vertexPosition;
in vec3 vertexColor;
uniform mat4 csMatrix;
uniform mat4 projMatrix;
out vec3 vertexColorToFS;

void main() {
    gl_Position = projMatrix * csMatrix * vec4(vertexPosition, 1);
    vertexColorToFS = vertexColor;
}
