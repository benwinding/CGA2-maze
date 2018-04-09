#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Player.h"

Player::Player(CubeMesh *cubeMesh)
{
    this->cubeMesh = cubeMesh;
    this->pan = 0;
    this->tilt = 0;
    this->turnIncrement = 1;
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
    this->pan -= turnIncrement;
}
void Player::PanRight()
{
    this->pan += turnIncrement;
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

