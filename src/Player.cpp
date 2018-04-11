#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Player.h"

Player::Player(CubeMesh *cubeMesh)
{
    this->cubeMesh = cubeMesh;
    this->Reset();
}

void Player::Reset()
{
    this->location = glm::ivec2(0,0);
    this->pan = 90;
    this->tilt = 90;
    this->turnIncrement = 3;
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
    if(this->pan <= 0)
        this->pan = 360;
}
void Player::PanRight()
{
    this->pan += turnIncrement;
    if(this->pan >= 360)
        this->pan = 0;
}
void Player::TiltUp()
{
    this->tilt -= turnIncrement; 
    if(this->tilt <= 30)
        this->tilt = 30;
}
void Player::TiltDown()
{
    this->tilt += turnIncrement; 
    if(this->tilt >= 150)
        this->tilt = 150;
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

