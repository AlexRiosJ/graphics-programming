#version 400

in vec2 texcoordVF;
out vec4 pixelColor; 

void main() {
   float d = length(texcoordVF - vec2(0.5, 0.5));
   if(d < 0.25) pixelColor = vec4(d, 1 - d, 0, 1.0);
   else if(d < 0.5) pixelColor = vec4(0, d, 1 - d, 1.0);
   else pixelColor = vec4(1 - d, 0, d, 1.0);
}
