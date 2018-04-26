#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Player.h"

Player::Player(CubeMesh *cubeMesh, DiamondMesh *diamondMesh, int mazeSize)
{
    this->mazeSize = mazeSize;
    this->cubeMesh = cubeMesh;
    this->diamondMesh = diamondMesh;
    this->hatEnabled = false;
    this->Reset();
}

void Player::Reset()
{
    this->location = glm::ivec2(0,0);
    this->pan = 90;
    this->tilt = 90;
    this->turnIncrement = 5;
}

float Player::GetPlayerHeight()
{
    static double startTime = glfwGetTime();
    double nowTime = startTime - glfwGetTime();
    return 1+0.04f*sin(nowTime);
}

void Player::renderPlayer(int shaderID)
{
    glm::ivec3 location3 = this->GetLocation3();
    float pan = this->GetPan();
    float tilt = this->GetTilt();

    // Render Player Vertical Section
    this->diamondMesh->Reset(shaderID);
    this->diamondMesh->Translate(location3.x, this->GetPlayerHeight(), location3.z);
    this->diamondMesh->RotateY(-pan);
    this->diamondMesh->RotateZ(-(tilt-90+15));
    this->diamondMesh->RotateY(-90);
    this->diamondMesh->Translate(0, 0.9f, -0.5f);
    this->diamondMesh->Scale(0.5f, 2, 0.5f);
    this->diamondMesh->Draw();

    glBindVertexArray(0);
    glFlush();
}

void Player::renderHat(int shaderID)
{
    if(this->hatEnabled == false)
        return;

    glm::ivec3 location3 = this->GetLocation3();
    float pan = this->GetPan();
    float tilt = this->GetTilt();

    // Render Player Hat
    this->cubeMesh->Reset(shaderID);
    this->cubeMesh->Translate(location3.x, this->GetPlayerHeight(), location3.z);
    this->cubeMesh->RotateY(-pan);
    this->cubeMesh->RotateZ(-(tilt-90+15));
    this->cubeMesh->RotateY(-90);
    this->cubeMesh->Translate(0, 0.9f, -0.5f);
    this->cubeMesh->Scale(0.4f, 0.1, 0.2f);
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

void Player::ToggleHat()
{
  this->hatEnabled = !this->hatEnabled;
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
glm::vec3 Player::GetLocation3()
{
    float i = this->location[0];
    float j = this->location[1];
    float x = i*2 - mazeSize + 1;
    float z = j*2 - mazeSize + 1;
    return glm::vec3((int)x, this->GetPlayerHeight(), (int)z);
}

glm::vec3 Player::GetDirection3()
{
    float pan = glm::radians(this->GetPan());
    float tilt = glm::radians(this->GetTilt());

    float camX = sin(tilt)*cos(pan);
    float camY = cos(tilt);
    float camZ = sin(tilt)*sin(pan);

    return glm::vec3(camX, camY, camZ);
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

