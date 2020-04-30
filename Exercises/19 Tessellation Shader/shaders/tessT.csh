#version 400

layout (vertices = 3) out;

uniform float cameraZ = 1.0;

in  vec4 patchPositionToTCS[]; 
in  vec3 patchColorToTCS[];
out vec4 patchPositionToTES[];
out vec3 patchColorToTES[];

void main() {
	patchPositionToTES[gl_InvocationID] = patchPositionToTCS[gl_InvocationID];
	patchColorToTES[gl_InvocationID]    = patchColorToTCS[gl_InvocationID];
	
	vec4 position  = (patchPositionToTES[0] + patchPositionToTES[1] + patchPositionToTES[2]) / 3;
	float distance = length(vec3(0, 0, cameraZ) - position.xyz);
	
	int maxLevel = 32;
	int tessLevel = clamp(maxLevel - int(distance), 2, maxLevel);
	
    gl_TessLevelInner[0] = tessLevel;
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
}

