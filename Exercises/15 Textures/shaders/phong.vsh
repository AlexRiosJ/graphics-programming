#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexcoord;

uniform mat4 modelMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

out vec3 worldVertexPosition;
out vec3 worldVertexNormal;
out vec2 vertexTexcoordToFS;

void main() {
    vec4 worldPosition = modelMatrix * vec4(vertexPosition, 1);
    gl_Position = projMatrix * viewMatrix * worldPosition;

    worldVertexPosition = worldPosition.xyz;

    mat4 G = transpose(inverse(modelMatrix));
    worldVertexNormal = (G * vec4(vertexNormal, 0)).xyz;

    vertexTexcoordToFS = vertexTexcoord;
}
