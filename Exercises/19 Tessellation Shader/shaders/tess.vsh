#version 330

in vec3 patchPosition;      
in vec3 patchColor;
out vec4 patchPositionToTCS;
out vec3 patchColorToTCS;

void main() {
   patchPositionToTCS = vec4(patchPosition, 1);
   patchColorToTCS    = patchColor;
}
