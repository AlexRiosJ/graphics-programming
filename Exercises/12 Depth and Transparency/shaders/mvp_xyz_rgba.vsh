#version 330

in vec3 vertexPosition;
in vec4 vertexColor;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
out vec4 vertexColorToFS;

void main() {
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1);
    vertexColorToFS = vertexColor;
}
