#version 330

in vec4 gl_FragCoord;
out vec4 pixelColor;

void main() {
    float red = (gl_FragCoord.x - (256 * floor(gl_FragCoord.x / 256))) / 256;
    float blue = (gl_FragCoord.y - (256 * floor(gl_FragCoord.y / 256))) / 256;
    float temp = gl_FragCoord.x * gl_FragCoord.y - gl_FragCoord.x;
    float green = (temp - (256 * floor(temp / 256))) / 256;
    pixelColor = vec4(red, green, blue, 1.0);
}