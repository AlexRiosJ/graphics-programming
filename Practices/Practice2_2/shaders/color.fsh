#version 330

in vec4 gl_FragCoord;
out vec4 pixelColor;

void main() {
    float red = float(int(gl_FragCoord.x) % 256) / 255;
    float blue = float(int(gl_FragCoord.y) % 256) / 255;
    float temp = gl_FragCoord.x * gl_FragCoord.y;
    float green = float(int(temp) % 256) / 255;
    pixelColor = vec4(red, green, blue, 1.0);
}