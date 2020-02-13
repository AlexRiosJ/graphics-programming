#version 330

in vec2 vertexPosition;
in vec3 vertexColor;
uniform float startX;

out vec3 vertexColorToFS;

void main() {
    gl_Position = vec4(vertexPosition.x + startX, vertexPosition.y, 0, 1);
    vertexColorToFS = vertexColor;
}