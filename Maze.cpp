#include "Maze.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define CUBE_NUM_VERTICES 8
#define CUBE_NUM_TRIS 12
#define VALS_PER_VERT 3

/**
 * Creates a new vertex array object for a cube
 * and loads in data into a vertex attribute buffer
 */
void Maze::createVAO(int programID)
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

    glUseProgram(programID);
	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle);

	unsigned int buffer[2];
	glGenBuffers(2, buffer);

	// Set vertex attribute
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

 	// Set element attributes. Notice the change to using GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(Indices), Indices, GL_STATIC_DRAW);   

    // Un-bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Maze::Maze(int gridRows, int gridCols, int* mazeLayout, int id) 
{
    float cubeSize = 2;

    this->gridRows = gridRows;
    this->gridCols = gridCols;

    this->wallThickness = 0.2;
    this->mazeX = gridRows;
    this->mazeZ = gridCols;
    this->mazeY = cubeSize * wallThickness;

    this->mazeLayout = mazeLayout;
    this->createVAO(id);
}

void drawCube(int modelUniformHandle, float tx, float ty, float tz, float sx, float sy, float sz)
{
    glm::mat4 model;
    model = glm::mat4();
    model = glm::translate( model, glm::vec3(tx,ty,tz));
    model = glm::scale( model, glm::vec3(sx,sy,sz));
    glUniformMatrix4fv( modelUniformHandle, 1, false, glm::value_ptr(model) );
    glDrawElements(GL_TRIANGLES, CUBE_NUM_TRIS*3, GL_UNSIGNED_INT, 0);    
}

/**
 Draw the table as a set of scaled blocks.
*/  
void Maze::render(int programID)
{
	glUseProgram(programID);

	int modelUniformHandle = glGetUniformLocation(programID, "model");
	if (modelUniformHandle == -1)
		exit(1);

 	glBindVertexArray(this->vaoHandle);   

    // We have separated modelling from viewing.
    // Viewing (i.e. placing the camera relative to the entire table)
    // is handled in the viewer class.
    glm::mat4 model;

    // now apply the scales to each cube forming the table
    // First the floor
    drawCube(modelUniformHandle, 0, -wallThickness, 0, mazeX, wallThickness, mazeZ);
    drawCube(modelUniformHandle, 0, -(wallThickness-1), mazeZ+wallThickness, mazeX+2*wallThickness, wallThickness+1, wallThickness);
    drawCube(modelUniformHandle, 0, -(wallThickness-1), -mazeZ-wallThickness, mazeX+2*wallThickness, wallThickness+1, wallThickness);
    drawCube(modelUniformHandle, mazeX+wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeZ);
    drawCube(modelUniformHandle, -mazeX-wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeZ);

    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    for(int i=0; i<sizeI; i++)
    {
    	for (int j=0; j<sizeJ; ++j)
    	{
    		int gridValue = mazeLayout[i*sizeJ+j];
            if(gridValue == 1) {
                drawCube(modelUniformHandle, i*2 - mazeX + 1, 1, j*2 - mazeZ + 1, 1, 1, 1);
            }
            if(gridValue == 2) {
                drawCube(modelUniformHandle, i*2 - mazeX + 1, 1, j*2 - mazeZ + 1, 0.2, 0.2, 0.2);
            }
    	}
    }
	glBindVertexArray(0);

	glFlush();
}

void Maze::SetPosition(int currentX, int currentY, float currentAngle) 
{
    this->currentX = currentX;
    this->currentY = currentY;
    this->currentAngle = currentAngle;
}

int Maze::GetCurrentX() 
{
    return this->currentX;
}

int Maze::GetCurrentY() 
{
    return this->currentY;
}

float Maze::GetCurrentAngle() 
{
    return this->currentAngle;
}

bool Maze::IsCollision(int i, int j)
{
    // Render Current Maze Layout
    int sizeJ = this->gridCols;
    int gridValue = mazeLayout[i*sizeJ+j];
    if(gridValue == 1)
        return true;
    else
        return false;
}
