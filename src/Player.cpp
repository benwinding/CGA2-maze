#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Player.h"

Player::Player(CubeMesh *cubeMesh)
{
    this->cubeMesh = cubeMesh;
}

void Player::renderPlayer(int shaderID)
{
    glUseProgram(shaderID);
    int modelUniformHandle = glGetUniformLocation(shaderID, "model");
    if (modelUniformHandle == -1)
        exit(1);

    glBindVertexArray(this->cubeMesh->getCubeVAOHandle());   

    float i = this->location[0];
    float j = this->location[1];
    this->cubeMesh->drawCube(modelUniformHandle, i, 1, j, 0.1, 10, 0.1);
    glBindVertexArray(0);

    glFlush();
}

// Setters
void Player::SetLocation(glm::ivec2 newLocation)
{
  this->location = newLocation;
}

void Player::SetPan(float pan)
{
  this->pan = pan;
}

void Player::SetTilt(float tilt)
{
  this->tilt = tilt;
}

// Move commands
void Player::PanLeft()
{
    this->pan += turnIncrement;
}
void Player::PanRight()
{
    this->pan -= turnIncrement;
}
void Player::TiltUp()
{
    this->tilt += turnIncrement; 
}
void Player::TiltDown()
{
    this->tilt -= turnIncrement; 
}

// Getters
glm::ivec2 Player::GetLocation()
{
  return this->location;
}

float Player::GetPan()
{
  return this->pan;
}

float Player::GetTilt()
{
  return this->tilt;
}

