#include "Viewer.h"
#include "glm/gtc/matrix_transform.hpp"

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
    glm::vec3 location = thePlayer.GetLocation3();
    glm::vec3 at(location.x, location.y, location.z);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFocus = thePlayer.GetDirection3();

    glm::vec3 lookBoth = at + cameraFocus;

    viewMtx = glm::lookAt(at, lookBoth, up);
}
