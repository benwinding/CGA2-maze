#ifndef MAZE__H
#define MAZE__H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CubeMesh.h"
#include "DiamondMesh.h"

class Maze
{
private:
    int gridRows, gridCols;
    int* mazeLayout;
    int mazeSize;
    float mazeY;
    float wallThickness;

    CubeMesh *cubeMesh;
    DiamondMesh *diamondMesh;

    int getLocationValue(glm::ivec2 pos);
    glm::vec3 getGridLocation(int i, int j);
    glm::vec3 goalLoaction;

public:
    Maze(CubeMesh *cubeMesh, DiamondMesh *diamondMesh);
    void SetUpMaze(int gridRows, int gridCols, int* mazeLayout);
    bool IsLocationWall(glm::ivec2 pos);
    bool IsLocationGoal(glm::ivec2 pos);

    void renderMazeBoundary(int shaderID);
    void renderMazeFloor(int shaderID);
    void renderWalls(int shaderID);
    void renderGoal(int shaderID);

    int GetMazeSize();
    void setGoalLocation(int i, int j);
    glm::vec3 GetGoalLocation3();
};

#endif