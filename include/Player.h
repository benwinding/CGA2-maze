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

    glm::ivec2 location;

public:
    Player(CubeMesh *cubeMesh);

    glm::ivec2 GetLocation();
    float GetPan();
    float GetTilt();
    void renderPlayer(int shaderID);

    void SetLocation(glm::ivec2 newLocation);
    void SetPan(float pan);
    void SetTilt(float tilt);

    void PanLeft();
    void PanRight();
    void TiltUp();
    void TiltDown();
};

#endif