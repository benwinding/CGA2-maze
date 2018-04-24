#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "DiamondMesh.h"

#define CUBE_NUM_VERTICES 24
#define CUBE_NUM_TRIS 8
#define VALS_PER_VERT 8

#define DEG2RAD(x) ((x)*M_PI/180.0) 

DiamondMesh::DiamondMesh()
{
    createDiamondVAO();
}

int DiamondMesh::getDiamondVAOHandle()
{
    return this->diamondVAOHandle;
}

void DiamondMesh::Reset(int shaderID)
{
    this->model = glm::mat4();
    this->shaderID = shaderID;
    this->textureScale = glm::vec2(1.f);
    glBindVertexArray(this->getDiamondVAOHandle());
}

void DiamondMesh::Translate(float tx, float ty, float tz)
{
    this->model = glm::translate(this->model, glm::vec3(tx, ty, tz));
}

void DiamondMesh::Scale(float sx, float sy, float sz)
{
    this->model = glm::scale(this->model, glm::vec3(sx, sy, sz));
}

void DiamondMesh::RotateX(float rx)
{
    float rRads = DEG2RAD(rx);
    this->model = glm::rotate(this->model, rRads, glm::vec3(1, 0, 0));
}

void DiamondMesh::RotateY(float ry)
{
    float rRads = DEG2RAD(ry);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 1, 0));
}

void DiamondMesh::RotateZ(float rz)
{
    float rRads = DEG2RAD(rz);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 0, 1));
}

void DiamondMesh::Draw()
{
    int modelUniformHandle = glGetUniformLocation(shaderID, "model");
    if (modelUniformHandle == -1)
        exit(1);
    glUniformMatrix4fv(modelUniformHandle, 1, false, glm::value_ptr(model));

    this->scaleUniformHandle = glGetUniformLocation(shaderID, "scale");
    if (scaleUniformHandle == -1)
        exit(1);
    glUniform2fv(this->scaleUniformHandle, 1, glm::value_ptr(this->textureScale));

    glDrawArrays(GL_TRIANGLES, 0, CUBE_NUM_VERTICES);
}

void DiamondMesh::MakeDiamond(int shaderID, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz)
{
    this->Reset(shaderID);
    this->Translate(tx,ty,tz);
    this->Scale(sx,sy,sz);
}

void DiamondMesh::SetTextureScale(float s, float t)
{
    this->textureScale = glm::vec2(s, t);
}

void DiamondMesh::createDiamondVAO()
{
    float Vertices[] = {
        // Positions            // Normals           // Texture Coords
        // far front
        -1.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, // 1
        1.0f,   0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  0.0f, // 2
        0.0f,   1.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f, // 3

        -1.0f,  0.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f, // 1
        1.0f,   0.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f, // 2
        0.0f,  -1.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f, // 3
        
        // near front
        1.0f,   0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f, // 1
        -1.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f, // 2
        0.0f,   1.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f, // 3

        1.0f,   0.0f,  1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, // 1
        -1.0f,  0.0f,  1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  0.0f, // 2
        0.0f,  -1.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f, // 3

        // left
        -1.0f,  0.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a
        -1.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 1
         0.0f,  1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4

        -1.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4
        -1.0f,  0.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // d
         0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a

        // left
         1.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a
         1.0f,  0.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 1
         0.0f,  1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4

         1.0f,  0.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4
         1.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // d
         0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a
    };

    glGenVertexArrays(1, &this->diamondVAOHandle);
    glBindVertexArray(this->diamondVAOHandle);

    unsigned int buffer[3];
    glGenBuffers(3, buffer);

    // Set vertex attribute Postion
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 0));

    // Set vertex attribute Normal
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 3));

    // Set vertex attribute Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 6));

    // Un-bind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

