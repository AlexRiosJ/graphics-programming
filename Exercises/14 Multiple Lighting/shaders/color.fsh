#version 400

in vec3 vertexColorVF;

out vec4 pixelColor; 

void main() {
	pixelColor = vec4(vertexColorVF, 1.0);
}