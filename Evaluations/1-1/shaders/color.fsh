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

    vec3 color1 = vec3(0.04, 0.16, 0.08);
    vec3 color3 = vec3(0.67, 0.17, 0.17);
    vec3 color2 = vec3(0.90, 0.98, 0.67);


    vec2 x = vec2(-1.2 + i * delta, -1.2 + j * delta);
    vec2 z = vec2(-0.74543, 0.11301);

    float count = 0;
    while(length(x) <= 2.0 && count < 128)
    {
        x = square(x) + z;
        count++;
    }

    vec3 finalColor;

    if(length(x) <= 2.0)
    {
        finalColor = color1;
    }
    else if(count == 128)
    {
        finalColor = color2;
    }
    else if(count == 0)
    {
        finalColor = color3;
    }
    else
    {
        float relativeCount = count / 128;
        finalColor = relativeCount * color2 + (1 - relativeCount) * color3;
    }

    pixelColor = vec4(finalColor, 1);
}