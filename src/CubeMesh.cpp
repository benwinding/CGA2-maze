#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CubeMesh.h"

#define CUBE_NUM_VERTICES 36
#define CUBE_NUM_TRIS 12
#define VALS_PER_VERT 8

CubeMesh::CubeMesh()
{
    createCubeVAO();
}

int CubeMesh::getCubeVAOHandle()
{
    return this->cubeVAOHandle;
}

void CubeMesh::Reset(int shaderID)
{
    this->model = glm::mat4();
    this->shaderID = shaderID;
    this->textureScale = glm::vec2(1.f);
    glBindVertexArray(this->getCubeVAOHandle());
}

void CubeMesh::Translate(float tx, float ty, float tz)
{
    this->model = glm::translate(this->model, glm::vec3(tx, ty, tz));
}

void CubeMesh::Scale(float sx, float sy, float sz)
{
    this->model = glm::scale(this->model, glm::vec3(sx, sy, sz));
}

void CubeMesh::RotateX(float rx)
{
    float rRads = glm::radians(rx);
    this->model = glm::rotate(this->model, rRads, glm::vec3(1, 0, 0));
}

void CubeMesh::RotateY(float ry)
{
    float rRads = glm::radians(ry);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 1, 0));
}

void CubeMesh::RotateZ(float rz)
{
    float rRads = glm::radians(rz);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 0, 1));
}

void CubeMesh::Draw()
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

void CubeMesh::MakeCube(int shaderID, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz)
{
    this->Reset(shaderID);
    this->Translate(tx,ty,tz);
    this->Scale(sx,sy,sz);
}

void CubeMesh::SetTextureScale(float s, float t)
{
    this->textureScale = glm::vec2(s, t);
}

void CubeMesh::createCubeVAO()
{
    float Vertices[] = {
        // Positions            // Normals           // Texture Coords
        // far front
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, // 1
        1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  0.0f, // 2
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f, // 3
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f, // 3
        -1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  1.0f, // 4
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, // 1

        // near front
        1.0f,  -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f, // b
        -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f, // a
        -1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f, // d
        -1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f, // d
        1.0f,   1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  1.0f, // c
        1.0f,  -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f, // b

        // left
        -1.0f, -1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a
        -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 1
        -1.0f,  1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4
        -1.0f,  1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 4
        -1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // d
        -1.0f, -1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // a

        // right
        1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,   0.0f,  0.0f, // 2
        1.0f, -1.0f,  1.0f,    1.0f,  0.0f,  0.0f,   1.0f,  0.0f, // b
        1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,   1.0f,  1.0f, // c
        1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,   1.0f,  1.0f, // c
        1.0f,  1.0f, -1.0f,    1.0f,  0.0f,  0.0f,   0.0f,  1.0f, // 3
        1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,   0.0f,  0.0f, // 2 

        // floor
        -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   0.0f,  1.0f, // 1
        1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   1.0f,  1.0f, // 2
        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f,  0.0f, // b
        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f,  0.0f, // a
        -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   0.0f,  1.0f,

        //sky
        -1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  1.0f, // d
        1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  1.0f, // c
        1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f, // 3
        1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f, // 3
        -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  0.0f, // 4
        -1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  1.0f  // d
    };

    glGenVertexArrays(1, &this->cubeVAOHandle);
    glBindVertexArray(this->cubeVAOHandle);

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

