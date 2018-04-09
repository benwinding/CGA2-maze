#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Maze.h"
#include "App.h"

Maze::Maze(CubeMesh *cubeMesh)
{
    this->cubeMesh = cubeMesh;
}


void Maze::SetUpMaze(int gridRows, int gridCols, int* mazeLayout)
{
    float cubeSize = 2;

    this->gridRows = gridRows;
    this->gridCols = gridCols;

    this->wallThickness = 0.2;
    this->mazeX = gridRows;
    this->mazeZ = gridCols;
    this->mazeY = cubeSize * wallThickness;

    this->mazeLayout = mazeLayout;
}

/**
 Draw the table as a set of scaled blocks.
*/  
void Maze::renderWalls(int shaderID)
{
    glUseProgram(shaderID);
    int modelUniformHandle = glGetUniformLocation(shaderID, "model");
    if (modelUniformHandle == -1)
        exit(1);

    glBindVertexArray(this->cubeMesh->getCubeVAOHandle());

    // First the floor
    this->cubeMesh->drawCube(modelUniformHandle, 0, -wallThickness, 0, mazeX, wallThickness, mazeZ);
    this->cubeMesh->drawCube(modelUniformHandle, 0, -(wallThickness-1), mazeZ+wallThickness, mazeX+2*wallThickness, wallThickness+1, wallThickness);
    this->cubeMesh->drawCube(modelUniformHandle, 0, -(wallThickness-1), -mazeZ-wallThickness, mazeX+2*wallThickness, wallThickness+1, wallThickness);
    this->cubeMesh->drawCube(modelUniformHandle, mazeX+wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeZ);
    this->cubeMesh->drawCube(modelUniformHandle, -mazeX-wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeZ);

    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    for(int i=0; i<sizeI; i++)
    {
        for (int j=0; j<sizeJ; ++j)
        {
            int gridValue = mazeLayout[i*sizeJ+j];
            if(gridValue == 1) {
                this->cubeMesh->drawCube(modelUniformHandle, i*2 - mazeX + 1, 1, j*2 - mazeZ + 1, 1, 1, 1);
            }
        }
    }
    glBindVertexArray(0);
    glFlush();
}

void Maze::renderGoal(int shaderID)
{
    glUseProgram(shaderID);
    int modelUniformHandle = glGetUniformLocation(shaderID, "model");
    if (modelUniformHandle == -1)
        exit(1);

    glBindVertexArray(this->cubeMesh->getCubeVAOHandle());

    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    for(int i=0; i<sizeI; i++)
    {
    	for (int j=0; j<sizeJ; ++j)
    	{
    		int gridValue = mazeLayout[i*sizeJ+j];
            if(gridValue == 2) {
                this->cubeMesh->drawCube(modelUniformHandle, i*2 - mazeX + 1, 1, j*2 - mazeZ + 1, 0.2, 0.2, 0.2);
            }
    	}
    }
	glBindVertexArray(0);

	glFlush();
}

int Maze::getLocationValue(glm::ivec2 pos)
{
    int i = pos[0];
    int j = pos[1];
    int size = this->gridCols;
    std::cout << "Maze:: " << "i=" << i << ",j=" << j << ",size=" << size << std::endl;
    if(i > size || i < 0) {
        std::cout << "Maze:: " << "i out of bounds" << std::endl;
        return 1;
    }
    if(j > size || j < 0) {
        std::cout << "Maze:: " << "j out of bounds" << std::endl;
        return 1;
    }

    int gridValue = mazeLayout[i*size+j];
    return gridValue;
}

bool Maze::IsLocationClear(glm::ivec2 pos)
{
    return getLocationValue(pos) == 0;
}

bool Maze::IsLocationGoal(glm::ivec2 pos)
{
    return getLocationValue(pos) == 2;
}
