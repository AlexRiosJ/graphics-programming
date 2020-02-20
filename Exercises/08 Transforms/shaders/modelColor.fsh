#version 330

uniform vec3 squareColor;
out vec4 pixelColor;

void main() {
    pixelColor = vec4(squareColor, 1);
}