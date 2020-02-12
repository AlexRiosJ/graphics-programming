#version 330

uniform vec4 color;
uniform ivec2 mouse;
out vec4 pixelColor;

void main() {
    vec2 mouseXY = mouse;
    vec4 stain = vec4(distance(gl_FragCoord.xy, mouseXY) / 70, 1, 0, 1);
    if(distance(gl_FragCoord.xy, mouseXY) < 70) {
        pixelColor = color * 0.5 + stain * (1 - 0.5); // Merge of two colors
    } else {
        pixelColor = color;
    }
}