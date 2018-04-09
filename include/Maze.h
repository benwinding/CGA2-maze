#ifndef MAZE__H
#define MAZE__H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CubeMesh.h"
#include "Player.h"

class Maze
{
private:
    int gridRows, gridCols;
    int* mazeLayout;
    float mazeX, mazeY, mazeZ;
    float wallThickness;
    CubeMesh *cubeMesh;
    Player *thePlayer;

    int getLocationValue(glm::ivec2 pos);

public:
    Maze(CubeMesh *cubeMesh, Player *thePlayer);
    void SetUpMaze(int gridRows, int gridCols, int* mazeLayout);
    bool IsLocationClear(glm::ivec2 pos);
    bool IsLocationGoal(glm::ivec2 pos);

    void renderWalls(int shaderID);
    void renderGoal(int shaderID);
    void renderPlayer(int shaderID);
};

#endif