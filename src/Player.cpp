#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Player.h"

Player::Player(CubeMesh *cubeMesh, int mazeSize)
{
    this->mazeSize = mazeSize;
    this->cubeMesh = cubeMesh;
    this->Reset();
}

void Player::Reset()
{
    this->location = glm::ivec2(0,0);
    this->pan = 90;
    this->tilt = 90;
    this->turnIncrement = 5;
}

void Player::renderPlayer(int shaderID)
{
    glUseProgram(shaderID);

    glBindVertexArray(this->cubeMesh->getCubeVAOHandle());

    glm::ivec2 location = this->GetLocation();
    float i = location[0];
    float j = location[1];
    float x = i*2 - mazeSize + 1;
    float z = j*2 - mazeSize + 1;

    float pan = this->GetPan();
    float tilt = this->GetTilt();

    // Render Player Vertical Section
    this->cubeMesh->Reset(shaderID);
    this->cubeMesh->Translate(x, 0, z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->Scale(0.1, 4, 0.1);
    this->cubeMesh->Draw();

    // Render Player Direction Stick
    this->cubeMesh->Reset(shaderID);
    this->cubeMesh->Translate(x, 2, z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->RotateZ(-(tilt-90));
    this->cubeMesh->Scale(1, 0.1, 0.1);
    this->cubeMesh->Translate(1, 0, 0);
    this->cubeMesh->Draw();

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

