#include "sphere.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define RESET 0xFFFFFFFF

struct strSphere
{
	GLuint sphereVA;
	GLuint sphereBuffer[4];
	float radius;
	int parallels;
	int meridians;
	Vertex sphereColor;
	Vertex *vertices;
	Vertex *colors;
	Vertex *normals;
	GLuint *indexBuffer;
};

Sphere sphere_create(float radius, int parallels, int meridians, Vertex sphereColor)
{
	srand(time(NULL));
	int totalPositionVertices = parallels * (meridians + 1) * 2;
	int totalColors = parallels * (meridians + 1) * 2;
	int totalNormals = parallels * (meridians + 1) * 2;
	int totalIndexes = parallels * ((meridians + 1) * 2 + 1);
	Sphere sphere = (Sphere)malloc(sizeof(struct strSphere));
	sphere->radius = radius;
	sphere->parallels = parallels;
	sphere->meridians = meridians;
	sphere->sphereColor = sphereColor;
	sphere->vertices = (Vertex *)malloc(totalPositionVertices * sizeof(Vertex));
	sphere->colors = (Vertex *)malloc(totalColors * sizeof(Vertex));
	sphere->normals = (Vertex *)malloc(totalNormals * sizeof(Vertex));
	sphere->indexBuffer = (GLuint *)malloc(totalIndexes * sizeof(GLuint));
	float phi = 0;
	float theta = 0;
	float dPhi = M_PI / parallels;
	float dTheta = 2 * M_PI / meridians;
	int counter = 0;
	int indexCounter = 0;

	for (int i = 0; i < parallels; i++)
	{
		float randR = (float)rand() / RAND_MAX * 0.2;
		float randG = (float)rand() / RAND_MAX * 0.2;
		float randB = (float)rand() / RAND_MAX * 0.2;
		for (int j = 0; j < (meridians + 1) * 2; j += 2)
		{
			float x = sin(phi) * cos(theta) * radius;
			float y = sin(phi) * sin(theta) * radius;
			float z = cos(phi) * radius;

			sphere->normals[counter].x = sphere->vertices[counter].x = x;
			sphere->normals[counter].y = sphere->vertices[counter].y = y;
			sphere->normals[counter].z = sphere->vertices[counter].z = z;
			// printf("%d: %.2f, %.2f, %.2f\t", counter, sphere->vertices[counter].x, sphere->vertices[counter].y, sphere->vertices[counter].z);

			sphere->colors[counter].x = randR + sphereColor.x * 0.8;
			sphere->colors[counter].y = randG + sphereColor.y * 0.8;
			sphere->colors[counter].z = randB + sphereColor.z * 0.8;

			sphere->indexBuffer[indexCounter] = counter;

			// printf("%d: %.2f, %.2f, %.2f\t%d\n", counter, sphere->colors[counter].x, sphere->colors[counter].y, sphere->colors[counter].z, sphere->indexBuffer[indexCounter]);

			counter++;
			indexCounter++;

			x = sin(phi + dPhi) * cos(theta) * radius;
			y = sin(phi + dPhi) * sin(theta) * radius;
			z = cos(phi + dPhi) * radius;

			sphere->normals[counter].x = sphere->vertices[counter].x = x;
			sphere->normals[counter].y = sphere->vertices[counter].y = y;
			sphere->normals[counter].z = sphere->vertices[counter].z = z;
			// printf("%d: %.2f, %.2f, %.2f\t", counter, sphere->vertices[counter].x, sphere->vertices[counter].y, sphere->vertices[counter].z);

			sphere->colors[counter].x = randR + sphereColor.x * 0.8;
			sphere->colors[counter].y = randG + sphereColor.y * 0.8;
			sphere->colors[counter].z = randB + sphereColor.z * 0.8;

			sphere->indexBuffer[indexCounter] = counter;

			// printf("%d: %.2f, %.2f, %.2f\t%d\n", counter, sphere->colors[counter].x, sphere->colors[counter].y, sphere->colors[counter].z, sphere->indexBuffer[indexCounter]);

			counter++;
			indexCounter++;

			theta += dTheta;
		}
		
		theta = 0;
		phi += dPhi;
		sphere->indexBuffer[indexCounter] = RESET;

		// printf("%X\n", sphere->indexBuffer[indexCounter]);
		indexCounter++;
	}

	return sphere;
}

void sphere_bind(Sphere sphere, GLuint vertexPosLoc, GLuint vertexColLoc, GLuint vertexNormalLoc)
{
	int totalPositionVertices = sphere->parallels * (sphere->meridians + 1) * 2;
	int totalColors = sphere->parallels * (sphere->meridians + 1) * 2;
	int totalNormals = sphere->parallels * (sphere->meridians + 1) * 2;
	int totalIndexes = sphere->parallels * ((sphere->meridians + 1) * 2 + 1);

	glGenVertexArrays(1, &sphere->sphereVA);
	glBindVertexArray(sphere->sphereVA);
	glGenBuffers(4, sphere->sphereBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, sphere->sphereBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, totalPositionVertices * sizeof(Vertex), sphere->vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, sphere->sphereBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, totalColors * sizeof(Vertex), sphere->colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, sphere->sphereBuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, totalNormals * sizeof(Vertex), sphere->normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, sphere->sphereBuffer[3]);
	glBufferData(GL_ARRAY_BUFFER, totalIndexes * sizeof(GLuint), sphere->indexBuffer, GL_STATIC_DRAW);
	glPrimitiveRestartIndex(RESET);
	glEnable(GL_PRIMITIVE_RESTART);
}

void sphere_draw(Sphere sphere)
{
	glBindVertexArray(sphere->sphereVA);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere->sphereBuffer[3]);
	int totalIndexes = sphere->parallels * ((sphere->meridians + 1) * 2 + 1);
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexes * sizeof(GLuint), GL_UNSIGNED_INT, 0);
}