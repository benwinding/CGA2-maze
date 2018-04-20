#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "glm/gtc/matrix_transform.hpp"

#include "CubeMesh.h"

// Repsonsible For Tracking the players movements
class Player {
private:
    CubeMesh *cubeMesh;
    float turnIncrement;
    float pan, tilt;
    bool hatEnabled;

    glm::ivec2 location;
    int mazeSize;
public:
    Player(CubeMesh *cubeMesh, int mazeSize);
    void Reset();

    void renderPlayer(int shaderID);

    glm::ivec3 GetLocation3();
    glm::ivec2 GetGridLocation();
    float GetPan();
    float GetTilt();

    void SetLocation(glm::ivec2 newLocation);
    void SetPan(float pan);
    void SetTilt(float tilt);
    void ToggleHat();

    void PanLeft();
    void PanRight();
    void TiltUp();
    void TiltDown();
};

#endif