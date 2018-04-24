#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "glm/gtc/matrix_transform.hpp"

#include "CubeMesh.h"
#include "DiamondMesh.h"

// Repsonsible For Tracking the players movements
class Player {
private:
    CubeMesh *cubeMesh;
    DiamondMesh *diamondMesh;

    float turnIncrement;
    float pan, tilt;
    bool hatEnabled;

    glm::ivec2 location;
    int mazeSize;
public:
    Player(CubeMesh *cubeMesh, DiamondMesh *diamondMesh, int mazeSize);
    void Reset();

    void renderPlayer(int shaderID);
    void renderHat(int shaderID);

    glm::vec3 GetLocation3();
    glm::ivec2 GetGridLocation();
    glm::vec3 GetDirection3();
    float GetPan();
    float GetTilt();
    float GetPlayerHeight();

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