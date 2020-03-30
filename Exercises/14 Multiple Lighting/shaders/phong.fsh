#version 400

const int N = 3;

struct Light {
	vec3 lightColor;
	vec3 lightPosition;
	float exponent;
};

uniform LightBlock {
   Light lights[N];
};

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;

uniform vec3 cameraPosition;
uniform vec3 ambientLight;

uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;

out vec4 pixelColor;

void main() {
  vec3 n  = normalize(worldVertexNormal);
  vec3 tempPixelColor = ambientLight *  materialA;
  vec3 l, r;
  float factorD, factorS;
  vec3 v = normalize(cameraPosition - worldVertexPosition);

  int i;
  for(i = 0; i < N; i++) {    
  	l = normalize(lights[i].lightPosition - worldVertexPosition);  	
  	factorD = clamp(dot(l, n), 0, 1);
  	r = normalize((2 * n)  * dot(n, l) - l);
  	factorS = clamp(pow(dot(r, v), lights[i].exponent), 0, 1);
 	tempPixelColor += lights[i].lightColor * (materialD * factorD + materialS * factorS);
  }

  pixelColor = vec4(clamp(tempPixelColor, 0, 1), 1);
}

