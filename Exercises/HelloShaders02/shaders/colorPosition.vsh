#version 330

in vec2 vertexPosition;
in vec3 vertexColor;

out vec3 vertexColorToFS;

void main() {
    gl_Position = vec4(vec3(vertexPosition, 0) + vertexColor / 8, 1);
    vertexColorToFS = vertexColor;
}