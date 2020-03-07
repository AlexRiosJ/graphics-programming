#version 330

uniform int height;
uniform int width;
out vec4 pixelColor;

float vector_norm(vec2 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

vec2 square(vec2 vec)
{
    return vec2(vec[0] * vec[0] - vec[1] * vec[1], 2 * vec[0] * vec[1]);
}

void main() 
{
    float delta = 2.4 / width;
    float i = gl_FragCoord.x;
    float j = gl_FragCoord.y;

    vec2 x = vec2(-1.2 + i * delta, -1.2 + j * delta);
    vec2 z = vec2(-0.74543, 0.11301);

    float count = 0;

    while(length(x) <= 2.0 && count < 128)
    {
        x = square(x) + z;
        count++;
    }

    float grey;

    if(length(x) <= 2.0)
    {
        grey = 0;
    } else {
        grey = count / 128.0;
    }

    pixelColor = vec4(grey, grey, grey, 1);
}