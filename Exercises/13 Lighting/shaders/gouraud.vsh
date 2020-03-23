#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec3 ambientLight;
uniform vec3 materialA;

uniform vec3 diffuseLight;
uniform vec3 lightPosition;
uniform vec3 materialD;

out vec3 vertexColorToFS;

void main() {
    vec4 worldPosition = modelMatrix * vec4(vertexPosition, 1);
    vec4 worldNormal = modelMatrix * vec4(vertexNormal, 0);
    gl_Position = projMatrix * viewMatrix * worldPosition;
    
    // Modelo de Phong implementado a nivel de vertice (Gouraud)
    vec3 vertexToLight = normalize(lightPosition - worldPosition.xyz);
    float factorD = clamp(dot(vertexToLight, normalize(worldNormal.xyz)), 0.0, 1.0);

    vertexColorToFS = ambientLight * materialA + diffuseLight * (materialD * factorD);
}
