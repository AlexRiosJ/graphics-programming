#version 330

in vec3 vertexColorToFS;

out vec4 pixelColor;

//uniform ivec2 mouse;

void main() {
	pixelColor = vec4(vertexColorToFS, 1.0);
	
	/*
	if(distance(gl_FragCoord.xy, mouse) <= 50) discard;

	if(gl_FragCoord.y < 200) pixelColor = vec4(vertexColorToFS * 0.7, 1.0);
	else pixelColor = vec4(vertexColorToFS, 1.0);*/
}
