#version 330

in vec4 vertexColorToFS;
out vec4 pixelColor;

void main() {
	pixelColor = vertexColorToFS;
}
