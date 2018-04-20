#include <stdio.h>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "glm/gtx/string_cast.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "App.h"
#include "Shader.h"

App::App(int winX, int winY, int mazeSize, int* mazeConfig)
{
    this->SetWindowSize(winX, winY);
    
    this->cubeMesh = new CubeMesh();
    this->ThePlayer = new Player(this->cubeMesh, mazeSize);
    this->TheMaze = new Maze(this->cubeMesh, this->ThePlayer);
    
    this->TheMaze->SetUpMaze(mazeSize, mazeSize, mazeConfig);

    this->ObjCam = new ObjectViewer(glm::vec3(0,mazeSize*2,0.5));
    this->PlayerCam = new PlayerViewer(glm::vec3(0,0,0), this->TheMaze->GetMazeSize());
    this->Camera = PlayerCam;

    this->TextureGround = new Texture("res/star_wars_trench.png");
    this->TextureWalls = new Texture("res/crate.png");
    this->TextureHat = new Texture("res/hat.png");
    this->NoTexture = new Texture("res/white.png");

    this->TexturesOn = true;

    this->SetShaders();
}

App::~App()
{
    delete this->cubeMesh;
    delete this->TheMaze;
    delete this->ThePlayer;
    delete this->ObjCam;
    delete this->PlayerCam;
    delete this->wallsShader;
}

void App::SetShaders()
{
    std::string prefix = "res/shaders/";
    // Set up the shaders we are to use. 0 indicates error.
    this->wallsShader = new Shader(prefix + "walls.vert", prefix + "walls.frag");
}

void App::render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(*ThePlayer);

    int currentId;
    currentId = wallsShader->GetId();
    wallsShader->use();
    wallsShader->setMat4("projection", this->projection);
    wallsShader->setMat4("view", Camera->getViewMtx());
    wallsShader->setVec3("lightPos", ThePlayer->GetLocation3());

    NoTexture->Use();
    if(this->TexturesOn)
        TextureGround->Use();
    wallsShader->setRgb("objectColor", 100, 100, 100);
    TheMaze->renderMazeBoundaries(currentId);
    if(this->TexturesOn)
        TextureWalls->Use();
    wallsShader->setRgb("objectColor", 124, 68, 42);
    TheMaze->renderWalls(currentId);

    if(this->TexturesOn)
        TextureHat->Use();
    wallsShader->setRgb("objectColor", 0, 255, 0);
    ThePlayer->renderPlayer(currentId);
    NoTexture->Use();
    wallsShader->setRgb("objectColor", 255, 0, 0);
    TheMaze->renderGoal(currentId);
}

void App::MoveStraight(float moveAngle)
{
    float newPan = moveAngle + ThePlayer->GetPan();
    int index = (int)round(newPan / 90.f) % 4;
    glm::ivec2 vecArray[4] = {
        glm::ivec2(-1,0),
        glm::ivec2(0,-1),
        glm::ivec2(1,0),
        glm::ivec2(0,1),
    };
    glm::ivec2 moveVector = vecArray[index];
    glm::ivec2 newLocation = ThePlayer->GetGridLocation() + moveVector;
    if(TheMaze->IsLocationWall(newLocation))
        return;
    if(TheMaze->IsLocationGoal(newLocation))
        ThePlayer->Reset();
    else
        ThePlayer->SetLocation(newLocation);
}

void App::key_callback(int key, int action)
{
    if (action == GLFW_PRESS) 
    {
        switch(key) 
        {
            case '1':
                Camera = PlayerCam;
                break;
            case '2':
                Camera = ObjCam;
                break;
            case GLFW_KEY_T:
                toggleTextures();
                break;
            case GLFW_KEY_H:
                this->ThePlayer->ToggleHat();
                break;
            case GLFW_KEY_UP:
                this->MoveStraight(180);
                break;
            case GLFW_KEY_DOWN:
                this->MoveStraight(0);
                break;
        }
    }
    switch(key) 
    {
        case GLFW_KEY_LEFT:
            ThePlayer->PanLeft();
            break;
        case GLFW_KEY_RIGHT:
            ThePlayer->PanRight();
            break;
        case GLFW_KEY_A:
            ThePlayer->TiltUp();
            break;
        case GLFW_KEY_Z:
            ThePlayer->TiltDown();
            break;
        default:
            break;
    }
}   

void App::toggleTextures()
{
    this->TexturesOn = !this->TexturesOn;
    std::cout << "Textures Toggled, value: " << this->TexturesOn << std::endl;
}

int App::SetWindowSize(int x, int y)
{
    this->winX = x;
    this->winY = y;
    this->updateProjection();
}

void App::updateProjection()
{
    float fov = (float) M_PI / 2.f;
    this->projection = glm::perspective(fov, (float) winX / winY, 0.5f, 10000.0f );
}    
