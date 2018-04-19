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
    glm::ivec3 location3 = this->GetLocation3();
    float pan = this->GetPan();
    float tilt = this->GetTilt();

    // Render Player Vertical Section
    this->cubeMesh->Reset(shaderID);
    this->cubeMesh->Translate(location3.x, 0, location3.z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->Scale(0.1, 4, 0.1);
    this->cubeMesh->Draw();

    // Render Player Direction Stick
    this->cubeMesh->Reset(shaderID);
    this->cubeMesh->Translate(location3.x, 2, location3.z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->RotateZ(-(tilt-90));
    this->cubeMesh->Scale(0.7, 0.1, 0.6);
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
glm::ivec3 Player::GetLocation3()
{
    float i = this->location[0];
    float j = this->location[1];
    float x = i*2 - mazeSize + 1;
    float z = j*2 - mazeSize + 1;
    return glm::vec3(x, 1, z);
}

glm::ivec2 Player::GetGridLocation()
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

