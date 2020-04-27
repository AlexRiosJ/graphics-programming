#version 400

uniform mat4 projectionMatrix, modelMatrix, viewMatrix;
uniform bool shot;

in vec3 vertexPositionToGS[];
in vec3 vertexNormalToGS[];
in vec3 vertexColorToGS[];
out vec3 colorToFS;

layout(triangles, invocations = 2) in;
layout(triangle_strip, max_vertices = 12) out;

void main() {
	for(int i = 0; i < vertexPositionToGS.length(); i ++) {
		vec3 vp = vertexPositionToGS[i];
		colorToFS = vertexColorToGS[i];
		if(shot) {
			vp = vp * 0.8 + vertexNormalToGS[i] * 0.2;
			colorToFS /= 2;
		}
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1);
		EmitVertex();
	}
	EndPrimitive();

	if(shot) {
		for(int i = 0; i < vertexPositionToGS.length(); i ++) {
			vec3 vp = vertexPositionToGS[i];
			vp = vp * 0.4 + vertexNormalToGS[i] * 0.6;
			colorToFS = 0.65 * vertexColorToGS[i];
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1);
			EmitVertex();
		}
	}
	EndPrimitive();
}