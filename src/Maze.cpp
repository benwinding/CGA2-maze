#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Maze.h"
#include "App.h"
#include "Player.h"

#define DEG2RAD(x) ((x)*M_PI/180.0) 

Maze::Maze(CubeMesh *cubeMesh, Player *thePlayer)
{
    this->cubeMesh = cubeMesh;
    this->thePlayer = thePlayer;
}

void Maze::SetUpMaze(int gridRows, int gridCols, int* mazeLayout)
{
    float cubeSize = 2;

    this->gridRows = gridRows;
    this->gridCols = gridCols;

    this->wallThickness = 0.2;
    this->mazeSize = gridRows;
    this->mazeSize = gridCols;
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
    this->cubeMesh->drawCube(modelUniformHandle, 0, -wallThickness, 0, mazeSize, wallThickness, mazeSize);
    this->cubeMesh->drawCube(modelUniformHandle, 0, -(wallThickness-1), mazeSize+wallThickness, mazeSize+2*wallThickness, wallThickness+1, wallThickness);
    this->cubeMesh->drawCube(modelUniformHandle, 0, -(wallThickness-1), -mazeSize-wallThickness, mazeSize+2*wallThickness, wallThickness+1, wallThickness);
    this->cubeMesh->drawCube(modelUniformHandle, mazeSize+wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeSize);
    this->cubeMesh->drawCube(modelUniformHandle, -mazeSize-wallThickness, -(wallThickness-1), 0, wallThickness, wallThickness+1, mazeSize);

    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    for(int i=0; i<sizeI; i++)
    {
        for (int j=0; j<sizeJ; ++j)
        {
            int gridValue = mazeLayout[i*sizeJ+j];
            if(gridValue == 1) {
                float x = i*2 - mazeSize + 1;
                float z = j*2 - mazeSize + 1;
                this->cubeMesh->Reset();
                this->cubeMesh->Translate(x, 1, z);
                this->cubeMesh->Draw(modelUniformHandle);
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
            if(gridValue == 2)
            {
                float x = i*2 - mazeSize + 1;
                float z = j*2 - mazeSize + 1;
                this->cubeMesh->Reset();
                this->cubeMesh->Translate(x, 1, z);
                this->cubeMesh->Scale(0.2, 0.2, 0.2);
                this->cubeMesh->Draw(modelUniformHandle);
            }
    	}
    }
	glBindVertexArray(0);
	glFlush();
}

void Log(std::string varName, float varVal)
{
    std::cout << varName << "=" << varVal << std::endl;
}

void Maze::renderPlayer(int shaderID)
{
    glUseProgram(shaderID);
    int modelUniformHandle = glGetUniformLocation(shaderID, "model");
    if (modelUniformHandle == -1)
        exit(1);

    glBindVertexArray(this->cubeMesh->getCubeVAOHandle());

    glm::ivec2 location = this->thePlayer->GetLocation();
    float i = location[0];
    float j = location[1];
    float x = i*2 - mazeSize + 1;
    float z = j*2 - mazeSize + 1;

    float pan = this->thePlayer->GetPan();

    this->cubeMesh->Reset();
    this->cubeMesh->Translate(x, 0, z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->Scale(0.1, 4, 0.1);
    this->cubeMesh->Draw(modelUniformHandle);

    this->cubeMesh->Reset();
    this->cubeMesh->Translate(x, 2, z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->Scale(1, 0.1, 0.1);
    this->cubeMesh->Translate(1, 0, 0);
    this->cubeMesh->Draw(modelUniformHandle);

    glBindVertexArray(0);
    glFlush();
}

int Maze::getLocationValue(glm::ivec2 pos)
{
    int i = pos[0];
    int j = pos[1];
    int size = this->gridCols;
    if(i >= size || i < 0) {
        std::cout << "Maze:: " << "i out of bounds" << "i=" << i << ",j=" << j << ",size=" << size << std::endl;
        return 1;
    }
    if(j >= size || j < 0) {
        std::cout << "Maze:: " << "j out of bounds" << "i=" << i << ",j=" << j << ",size=" << size << std::endl;
        return 1;
    }
    int gridValue = mazeLayout[i*size+j];
    return gridValue;
}

bool Maze::IsLocationWall(glm::ivec2 pos)
{
    return getLocationValue(pos) == 1;
}

bool Maze::IsLocationGoal(glm::ivec2 pos)
{
    return getLocationValue(pos) == 2;
}

int Maze::GetMazeSize()
{
    return this->mazeSize;
}
