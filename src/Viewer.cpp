
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "Viewer.h"
#include "glm/gtc/matrix_transform.hpp"

#define DEG2RAD(x) ((x)*M_PI/180.0) 
#define RAD2DEG(x) ((x)*180.0/M_PI) 

Viewer::Viewer( glm::vec3 eye )
{
    initEye = eye;
}

Viewer::~Viewer(){}

/**
 Assumes the current matrix has been calculated (usually in 
 update() or reset())
*/
const glm::mat4 Viewer::getViewMtx() const
{
    return viewMtx;
}

/**
 Resets the view matrix to the value the camera was 
 initialised with. Assumes looking at the origin.
*/
void Viewer::reset()
{
    glm::vec3 at(0.0f, 1.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(initEye, at, up);
}

ObjectViewer::ObjectViewer(glm::vec3 eye)
    : Viewer(eye)
{
    reset();
}

void ObjectViewer::update( Player &thePlayer ) 
{
}

// Player Viewer
PlayerViewer::PlayerViewer(glm::vec3 eye, int mazeSize)
    : Viewer(eye)
{
    this->mazeSize = mazeSize;
    reset();
}

void PlayerViewer::update(Player &thePlayer)
{
    glm::ivec2 location = thePlayer.GetLocation();
    float pan = DEG2RAD(thePlayer.GetPan());
    float tilt = DEG2RAD(thePlayer.GetTilt());

    int i = location[0];
    int j = location[1];

    float x = i*2 - this->mazeSize + 1;
    float z = j*2 - this->mazeSize + 1;

    float camX = sin(tilt)*cos(pan);
    float camY = cos(tilt);
    float camZ = sin(tilt)*sin(pan);

    glm::vec3 at(x, 1.0f, z);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFocus(camX, camY, camZ);

    glm::vec3 lookBoth = at + cameraFocus;

    viewMtx = glm::lookAt(at, lookBoth, up);
}
