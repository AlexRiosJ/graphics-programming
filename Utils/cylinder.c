#include "cylinder.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define RESET 0xFFFFFFFF

struct strCylinder
{
    GLuint cylinderVA;
    GLuint cylinderBuffer[4];
    float length;
    float bottomRadius;
    float topRadius;
    int slices;
    int stacks;
    Vertex bottomColor;
    Vertex topColor;
    Vertex *vertices;
    Vertex *colors;
    Vertex *normals;
    GLuint *indexBuffer;
};

Cylinder cylinder_create(float length, float bottomRadius, float topRadius, int slices, int stacks, Vertex bottomColor, Vertex topColor)
{
    srand(time(NULL));
    int totalPositionVertices = (slices * 2) + (slices + 1) * 2 * stacks;
    int totalColors = (slices * 2) + (slices + 1) * 2 * stacks;
    int totalNormals = (slices * 2) + (slices + 1) * 2 * stacks;
    int totalIndexes = stacks * ((slices + 1) * 2 + 1);
    Cylinder cylinder = (Cylinder)malloc(sizeof(struct strCylinder));
    cylinder->length = length;
    cylinder->bottomRadius = bottomRadius;
    cylinder->topRadius = topRadius;
    cylinder->slices = slices;
    cylinder->stacks = stacks;
    cylinder->bottomColor = bottomColor;
    cylinder->topColor = topColor;
    cylinder->vertices = (Vertex *)malloc(totalPositionVertices * sizeof(Vertex));
    cylinder->colors = (Vertex *)malloc(totalColors * sizeof(Vertex));
    cylinder->normals = (Vertex *)malloc(totalNormals * sizeof(Vertex));
    cylinder->indexBuffer = (GLuint *)malloc(totalIndexes * sizeof(GLuint));
    float theta = 0;
    float dTheta = 2 * M_PI / slices;
    int counter = 0;
    int indexCounter = 0;

    // Bottom Cover
    for (int i = 0; i < slices; i++)
    {
        float x = bottomRadius * cos(theta);
        float y = -length / 2;
        float z = bottomRadius * sin(theta);

        cylinder->vertices[counter].x = x;
        cylinder->vertices[counter].y = y;
        cylinder->vertices[counter].z = z;

        cylinder->colors[counter].x = bottomColor.x;
        cylinder->colors[counter].y = bottomColor.y;
        cylinder->colors[counter].z = bottomColor.z;

        cylinder->normals[counter].x = 0;
        cylinder->normals[counter].y = y;
        cylinder->normals[counter].z = 0;
        counter++;

        theta += dTheta;
    }

    // Sides
    float h = -length / 2;
    float dH = length / (float)stacks;

    float radius = bottomRadius;
    float dRadius = (topRadius - bottomRadius) / (float)stacks;

    Vertex color = bottomColor;
    Vertex dColor = {(topColor.x - bottomColor.x) / stacks, (topColor.y - bottomColor.y) / stacks, (topColor.z - bottomColor.z) / stacks};
    theta = 0;
    for (int i = 0; i < stacks; i++)
    {

        float randR = (float)rand() / RAND_MAX * 0.2;
        float randG = (float)rand() / RAND_MAX * 0.2;
        float randB = (float)rand() / RAND_MAX * 0.2;
        for (int j = 0; j < (slices + 1) * 2; j += 2)
        {
            float x = radius * cos(theta);
            float y = h;
            float z = radius * sin(theta);

            float yNormal = -(topRadius - bottomRadius);

            cylinder->vertices[counter].x = x;
            cylinder->vertices[counter].y = y;
            cylinder->vertices[counter].z = z;

            // printf("%d:\t%.2f\t%.2f\t%.2f\n", counter, cylinder->vertices[counter].x, cylinder->vertices[counter].y, cylinder->vertices[counter].z);

            cylinder->colors[counter].x = randR + color.x * 0.8;
            cylinder->colors[counter].y = randG + color.y * 0.8;
            cylinder->colors[counter].z = randB + color.z * 0.8;

            cylinder->normals[counter].x = x;
            cylinder->normals[counter].y = yNormal;
            cylinder->normals[counter].z = z;

            cylinder->indexBuffer[indexCounter] = counter;
            counter++;
            indexCounter++;

            x = (radius + dRadius) * cos(theta + dTheta);
            y = h + dH;
            z = (radius + dRadius) * sin(theta + dTheta);

            cylinder->vertices[counter].x = x;
            cylinder->vertices[counter].y = y;
            cylinder->vertices[counter].z = z;

            // printf("%d:\t%.2f\t%.2f\t%.2f\n", counter, cylinder->vertices[counter].x, cylinder->vertices[counter].y, cylinder->vertices[counter].z);

            cylinder->colors[counter].x = randR + color.x * 0.8;
            cylinder->colors[counter].y = randG + color.y * 0.8;
            cylinder->colors[counter].z = randB + color.z * 0.8;

            cylinder->normals[counter].x = x;
            cylinder->normals[counter].y = yNormal;
            cylinder->normals[counter].z = z;

            cylinder->indexBuffer[indexCounter] = counter;
            counter++;
            indexCounter++;
            theta += dTheta;
        }
        h += dH;
        radius += dRadius;
        theta = 0;
        color.x += dColor.x;
        color.y += dColor.y;
        color.z += dColor.z;
        cylinder->indexBuffer[indexCounter] = RESET;
        indexCounter++;
    }

    // Top Cover
    for (int i = 0; i < slices; i++)
    {
        float x = topRadius * cos(theta);
        float y = length / 2;
        float z = topRadius * sin(theta);

        cylinder->vertices[counter].x = x;
        cylinder->vertices[counter].y = y;
        cylinder->vertices[counter].z = z;

        cylinder->colors[counter].x = topColor.x;
        cylinder->colors[counter].y = topColor.y;
        cylinder->colors[counter].z = topColor.z;

        cylinder->normals[counter].x = 0;
        cylinder->normals[counter].y = y;
        cylinder->normals[counter].z = 0;
        counter++;

        theta += dTheta;
    }

    return cylinder;
}

void cylinder_bind(Cylinder cylinder, GLuint vertexPosLoc, GLuint vertexColLoc, GLuint vertexNormalLoc)
{
    int totalPositionVertices = (cylinder->slices * 2) + (cylinder->slices + 1) * 2 * cylinder->stacks;
    int totalColors = (cylinder->slices * 2) + (cylinder->slices + 1) * 2 * cylinder->stacks;
    int totalNormals = (cylinder->slices * 2) + (cylinder->slices + 1) * 2 * cylinder->stacks;
    int totalIndexes = cylinder->stacks * ((cylinder->slices + 1) * 2 + 1);

    glGenVertexArrays(1, &cylinder->cylinderVA);
    glBindVertexArray(cylinder->cylinderVA);
    glGenBuffers(4, cylinder->cylinderBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, cylinder->cylinderBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, totalPositionVertices * sizeof(Vertex), cylinder->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexPosLoc);
    glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinder->cylinderBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, totalColors * sizeof(Vertex), cylinder->colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexColLoc);
    glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinder->cylinderBuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, totalNormals * sizeof(Vertex), cylinder->normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexNormalLoc);
    glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinder->cylinderBuffer[3]);
    glBufferData(GL_ARRAY_BUFFER, totalIndexes * sizeof(GLuint), cylinder->indexBuffer, GL_STATIC_DRAW);
    glPrimitiveRestartIndex(RESET);
    glEnable(GL_PRIMITIVE_RESTART);
}

void cylinder_draw(Cylinder cylinder)
{
    glBindVertexArray(cylinder->cylinderVA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder->cylinderBuffer[3]);
    int totalIndexes = cylinder->stacks * ((cylinder->slices + 1) * 2 + 1);
    glDrawElements(GL_TRIANGLE_STRIP, totalIndexes * sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder->slices);
    glDrawArrays(GL_TRIANGLE_FAN, totalIndexes, cylinder->slices);
}