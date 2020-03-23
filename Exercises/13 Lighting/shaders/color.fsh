#version 330

uniform vec3 materialA;

out vec4 pixelColor;

void main() {
	pixelColor = vec4(materialA, 1);
}
