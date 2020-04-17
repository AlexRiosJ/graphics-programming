#version 330

in vec3 vertexPosition;
in vec3 vertexColor;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 vertexColorToFS;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1);
    vertexColorToFS = vertexColor;
}
