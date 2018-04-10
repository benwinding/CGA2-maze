#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CubeMesh.h"

#define CUBE_NUM_VERTICES 8
#define CUBE_NUM_TRIS 12
#define VALS_PER_VERT 3

#define DEG2RAD(x) ((x)*M_PI/180.0) 

CubeMesh::CubeMesh()
{
    createCubeVAO();
}

int CubeMesh::getCubeVAOHandle()
{
    return this->cubeVAOHandle;
}

void CubeMesh::Reset()
{
    this->model = glm::mat4();
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
    float rRads = DEG2RAD(rx);
    this->model = glm::rotate(this->model, rRads, glm::vec3(1, 0, 0));
}

void CubeMesh::RotateY(float ry)
{
    float rRads = DEG2RAD(ry);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 1, 0));
}

void CubeMesh::RotateZ(float rz)
{
    float rRads = DEG2RAD(rz);
    this->model = glm::rotate(this->model, rRads, glm::vec3(0, 0, 1));
}

void CubeMesh::Draw(int modelUniformHandle)
{
    glUniformMatrix4fv( modelUniformHandle, 1, false, glm::value_ptr(model) );
    glDrawElements(GL_TRIANGLES, CUBE_NUM_TRIS*3, GL_UNSIGNED_INT, 0);
}

void CubeMesh::drawCube(int modelUniformHandle, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz)
{
    this->Reset();
    this->Translate(tx,ty,tz);
    this->Scale(sx,sy,sz);
    this->Draw(modelUniformHandle);
}

void CubeMesh::createCubeVAO()
{
    // Cube vertex data
    float Vertices[ CUBE_NUM_VERTICES * VALS_PER_VERT ] = {
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f };

    // 12 triangles - 2 per face of the cube
    unsigned int Indices[CUBE_NUM_TRIS*3] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        5,4,7, 7,6,5,
        4,0,3, 3,7,4,
        3,2,6, 6,7,3,
        4,5,1, 1,0,4
    };

    glGenVertexArrays(1, &this->cubeVAOHandle);
    glBindVertexArray(this->cubeVAOHandle);

    unsigned int buffer[2];
    glGenBuffers(2, buffer);

    // Set vertex attribute
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

    // Set element attributes. Notice the change to using GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);   

    // Un-bind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

