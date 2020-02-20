#version 330

in vec2 vertexPosition;
uniform mat4 csMatrix;

void main() {
    gl_Position = csMatrix * vec4(vertexPosition, 0, 1);
}