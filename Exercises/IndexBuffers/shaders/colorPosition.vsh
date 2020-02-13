#version 330

in vec2 vertexPosition;
in vec3 vertexColor;

out vec3 vertexColorToFS;

void main() {
    gl_Position = vec4(vertexPosition, 0, 1);
    vertexColorToFS = vertexColor;
}