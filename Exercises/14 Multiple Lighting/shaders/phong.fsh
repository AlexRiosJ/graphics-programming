#version 400

struct Light {
	vec3 lightColor;
	vec3 lightPosition;
	float exponent;
};

const int N = 3;

uniform LightBlock {
   Light lights[N];
};

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;

uniform vec3 ambientLight;

uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;

uniform vec3 cameraPosition;

out vec4 pixelColor;

void main() {
	vec3 n = normalize(worldVertexNormal);
	vec3 temp = ambientLight * materialA;
	vec3 l, r;
	float factorD, factorS;
	vec3 v = normalize(cameraPosition - worldVertexPosition);

	for(int i = 0; i < N; i++) {
		l = normalize(lights[i].lightPosition - worldVertexPosition);
		factorD = clamp(dot(l, n), 0.0, 1.0);
		r = normalize((2 * n) * dot(n, l) - l);
		factorS = clamp(pow(dot(r, v), lights[i].exponent), 0.0, 1.0);
		temp += lights[i].lightColor * (materialD * factorD + materialS * factorS);
	}

	pixelColor = vec4(clamp(temp, 0.0, 1.0), 1);
}
