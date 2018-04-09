
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
PlayerViewer::PlayerViewer(glm::vec3 eye)
    : Viewer(eye)
{
    reset();
}

void PlayerViewer::update( Player &thePlayer ) 
{
    glm::ivec2 location = thePlayer.GetLocation();
    float pan = thePlayer.GetPan();
    float tilt = thePlayer.GetTilt();

    int x = location[0];
    int z = location[1];

    std::cout << "                   x: " << x << std::endl;
    std::cout << "                   z: " << z << std::endl;
    std::cout << "                tilt: " << tilt << std::endl;
    std::cout << "                 pan: " << pan << std::endl;
    std::cout << "  cos(DEG2RAD(tilt)): " << cos(DEG2RAD(tilt)) << std::endl;
    std::cout << "   sin(DEG2RAD(pan)): " << sin(DEG2RAD(pan)) << std::endl;

    glm::vec3 at(x, 1.0f, z);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    // glm::vec3 look(x + cos(DEG2RAD(tilt)), 1.0f, z + sin(DEG2RAD(pan)));
    glm::vec3 look(1.f,1.f,1.f);

    viewMtx = glm::lookAt(at, look, up);
}
