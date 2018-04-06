#ifndef MAZE__H
#define MAZE__H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Maze
{
    int gridRows, gridCols;
    float mazeX, mazeY, mazeZ;
    float wallThickness;

    float currentX, currentZ, currentAngle;

    int* mazeLayout;

	unsigned int vaoHandle;
	void createVAO();
public:
    float GetWidth();
    Maze(int gridRows, int gridCols, int* mazeLayout);
    void SetPosition(glm::vec3 currentPose);
    bool IsCollision(int i, int j);

    void renderWalls(int shaderID);
    void renderGoal(int shaderID);
    void renderPlayer(int shaderID);
};

#endif