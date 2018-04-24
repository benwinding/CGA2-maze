#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Maze.h"
#include "DiamondMesh.h"
#include "App.h"
#include "Player.h"

#define DEG2RAD(x) ((x)*M_PI/180.0) 

Maze::Maze(CubeMesh *cubeMesh, DiamondMesh *diamondMesh)
{
    this->cubeMesh = cubeMesh;
    this->diamondMesh = diamondMesh;
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
 Draw the maze boundry walls
*/  
void Maze::renderMazeBoundary(int shaderID)
{
    float wallHeight = 1.f+wallThickness;
    float wallLength1 = mazeSize+wallThickness;
    float wallLength2 = mazeSize+2*wallThickness;
    // North Wall
    this->cubeMesh->MakeCube(shaderID, 
        0, wallHeight, wallLength1, 
        wallLength2, wallHeight, wallThickness);
    this->cubeMesh->SetTextureScale(mazeSize, 1.f);
    this->cubeMesh->Draw();

    // South Wall
    this->cubeMesh->MakeCube(shaderID, 
        0, wallHeight, -wallLength1,
        wallLength2, wallHeight, wallThickness);
    this->cubeMesh->SetTextureScale(mazeSize, 1.f);
    this->cubeMesh->Draw();

    // East Wall
    this->cubeMesh->MakeCube(shaderID, 
        -wallLength1, wallHeight, 0,
        wallThickness, wallHeight, wallLength2);
    this->cubeMesh->SetTextureScale(mazeSize, 1.f);
    this->cubeMesh->Draw();

    // West Wall
    this->cubeMesh->MakeCube(shaderID, 
        wallLength1, wallHeight, 0,
        wallThickness, wallHeight, wallLength2);
    this->cubeMesh->SetTextureScale(mazeSize, 1.f);
    this->cubeMesh->Draw();

    glBindVertexArray(0);
    glFlush();
}

/**
 Draw the maze floor
*/  
void Maze::renderMazeFloor(int shaderID)
{
    // First the floor
    this->cubeMesh->MakeCube(shaderID, 0, -wallThickness, 0, mazeSize, wallThickness, mazeSize);
    this->cubeMesh->SetTextureScale(mazeSize, mazeSize);
    this->cubeMesh->Draw();
    glBindVertexArray(0);
    glFlush();
}

/**
 Draw the maze layout as a set of scaled blocks.
*/  
void Maze::renderWalls(int shaderID)
{
    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    for(int i=0; i<sizeI; i++)
    {
        for (int j=0; j<sizeJ; ++j)
        {
            int gridValue = mazeLayout[j*sizeI+i];
            if(gridValue == 1) {
                float x = i*2 - mazeSize + 1;
                float z = j*2 - mazeSize + 1;
                this->cubeMesh->Reset(shaderID);
                this->cubeMesh->Translate(x, 1, z);
                this->cubeMesh->Draw();
            }
        }
    }
    glBindVertexArray(0);
    glFlush();
}

void Maze::renderGoal(int shaderID)
{
    static double startTime = glfwGetTime();
    double nowTime = startTime - glfwGetTime();
    // Render Current Maze Layout
    int sizeI = this->gridRows;
    int sizeJ = this->gridCols;
    float goalHeight = 7 + 2*sin(nowTime) + 3*cos(nowTime*0.9f);
    for(int i=0; i<sizeI; i++)
    {
    	for (int j=0; j<sizeJ; ++j)
    	{
            int gridValue = mazeLayout[j*sizeI+i];
            if(gridValue == 2)
            {
                float x = i*2 - mazeSize + 1;
                float z = j*2 - mazeSize + 1;
                this->diamondMesh->Reset(shaderID);
                this->diamondMesh->Translate(x, goalHeight, z);
                this->diamondMesh->Scale(1, 4, 1);
                this->diamondMesh->RotateY(nowTime*20.f);
                this->diamondMesh->Draw();
            }
    	}
    }
	glBindVertexArray(0);
	glFlush();
}

int Maze::getLocationValue(glm::ivec2 pos)
{
    int i = pos.x;
    int j = pos.y;
    int size = this->gridCols;
    if(i >= size || i < 0) {
        std::cout << "Maze:: " << "i out of bounds" << "i=" << i << ",j=" << j << ",size=" << size << std::endl;
        return 1;
    }
    if(j >= size || j < 0) {
        std::cout << "Maze:: " << "j out of bounds" << "i=" << i << ",j=" << j << ",size=" << size << std::endl;
        return 1;
    }
    int gridValue = mazeLayout[j*size+i];
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
