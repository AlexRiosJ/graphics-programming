#version 330

in vec3 vertexColorToFS;
out vec4 pixelColor;
uniform vec3 factor;

uniform ivec2 mouse;

void main() {
    vec2 mouseXY = mouse;
    if(distance(gl_FragCoord.xy, mouseXY) < 20) discard;
    pixelColor = vec4(vertexColorToFS * factor, 1.0);
}