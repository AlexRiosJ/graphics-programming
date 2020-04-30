#version 400

layout (vertices = 4) out;

uniform float tessLevelInner = 1.0;
uniform float tessLevelOuter = 1.0;

in  vec4 patchPositionToTCS[]; 
in  vec3 patchColorToTCS[];
out vec4 patchPositionToTES[];
out vec3 patchColorToTES[];

void main() {
	patchPositionToTES[gl_InvocationID] = patchPositionToTCS[gl_InvocationID];
	patchColorToTES[gl_InvocationID]    = patchColorToTCS[gl_InvocationID];
    gl_TessLevelInner[0] = tessLevelInner;
    gl_TessLevelInner[1] = tessLevelInner;
    gl_TessLevelOuter[0] = tessLevelOuter;
    gl_TessLevelOuter[1] = tessLevelOuter;
    gl_TessLevelOuter[2] = tessLevelOuter;
    gl_TessLevelOuter[3] = tessLevelOuter;
}

