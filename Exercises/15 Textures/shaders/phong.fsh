#version 400

struct Light {
	vec3 lightColor;
	float subcutoff;
	vec3 lightPosition;
	float exponent;
	vec3 lightDirection;
	float cutoff;	
};

const int N = 3;
const float A = 1;
const float B = 0.1;
const float C = 0.01;

uniform LightBlock {
   Light lights[N];
};

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;
in vec2 vertexTexcoordToFS;

uniform vec3 ambientLight;

uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;
uniform sampler2D myTexture;

uniform vec3 cameraPosition;

out vec4 pixelColor;

void main() {
	vec3 n = normalize(worldVertexNormal);
	vec3 temp = ambientLight * materialA;
	vec3 l, r, d;
	float factorD, factorS, factorDeg, factorDist;
	vec3 v = normalize(cameraPosition - worldVertexPosition);

	for(int i = 0; i < N; i++) {
		l = normalize(lights[i].lightPosition - worldVertexPosition);
		d = normalize(lights[i].lightDirection);
		float cosAlpha = clamp(dot(d, -l), 0.0, 1.0);
		if(cosAlpha < lights[i].cutoff)
			continue;

		if(cosAlpha >= lights[i].subcutoff)
			factorDeg = 1.0;
		else
			factorDeg = (cosAlpha - lights[i].cutoff) / (lights[i].subcutoff - lights[i].cutoff);

		factorDist = length(worldVertexPosition - lights[i].lightPosition);
		float factorAtt = 1.0 / (A + B * factorDist + C * factorDist * factorDist);

		factorD = clamp(dot(l, n), 0.0, 1.0);
		r = normalize((2 * n) * dot(n, l) - l);
		factorS = pow(clamp(dot(r, v), 0.0, 1.0), lights[i].exponent);
		temp += factorDeg * factorAtt * lights[i].lightColor * (materialD * factorD + materialS * factorS);
	}

	pixelColor = vec4(clamp(temp, 0.0, 1.0), 1) * texture(myTexture, vertexTexcoordToFS);;
}
