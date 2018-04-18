#include <stdio.h>
#include <iostream>
#include <string.h>
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
    delete this->textureShader;
    delete this->plainShader;
}

void App::SetShaders()
{
    std::string prefix = "res/shaders/";
    // Set up the shaders we are to use. 0 indicates error.
    this->plainShader = new Shader(prefix + "plain.vert", prefix + "plain.frag");
    this->textureShader = new Shader(prefix + "walls.vert", prefix + "walls.frag");
}

void App::render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(*ThePlayer);

    if(this->TexturesOn)
    {
        textureShader->use();
        textureShader->setMat4("projection", this->projection);
        textureShader->setMat4("view", Camera->getViewMtx());
        textureShader->setVec3("lightPos", 1.f, 2.f, 0.f);

        TextureGround->Use();
        TheMaze->renderMazeBoundaries(textureShader->GetId());
        TextureWalls->Use();
        TheMaze->renderWalls(textureShader->GetId());

        TextureGround->DontUse();
        TextureWalls->DontUse();
        plainShader->use();
        plainShader->setMat4("projection", this->projection);
        plainShader->setMat4("view", Camera->getViewMtx());
        ThePlayer->renderPlayer(plainShader->GetId());
        TheMaze->renderGoal(plainShader->GetId());
    }
    else 
    {
        TextureGround->DontUse();
        TextureWalls->DontUse();
        plainShader->use();
        plainShader->setMat4("projection", this->projection);
        plainShader->setMat4("view", Camera->getViewMtx());
        
        TheMaze->renderMazeBoundaries(plainShader->GetId());
        TheMaze->renderWalls(plainShader->GetId());        
        ThePlayer->renderPlayer(plainShader->GetId());
        TheMaze->renderGoal(plainShader->GetId());
    }
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
    glm::ivec2 newLocation = ThePlayer->GetLocation() + moveVector;
    if(TheMaze->IsLocationWall(newLocation))
        return;
    if(TheMaze->IsLocationGoal(newLocation))
        ThePlayer->Reset();
    else
        ThePlayer->SetLocation(newLocation);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        switch(key) 
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case '1':
                Camera = PlayerCam;
                break;
            case '2':
                Camera = ObjCam;
                break;
            case GLFW_KEY_T:
                toggleTextures();
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
    this->projection = glm::perspective(fov, (float) winX / winY, 0.2f, 10000.0f );
}    
