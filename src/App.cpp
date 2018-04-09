#include <stdio.h>
#include <iostream>

#include <cmath>
#include "App.h"

App::App(int winX, int winY)
{
    this->winX = winX;
    this->winY = winY;
    this->cubeMesh = new CubeMesh();
    this->ThePlayer = new Player(this->cubeMesh);
    this->TheMaze = new Maze(this->cubeMesh, this->ThePlayer);

    this->ObjCam = new ObjectViewer(glm::vec3(0,5,15));
    this->PlayerCam = new PlayerViewer(glm::vec3(0,0,0));    
    this->Camera = PlayerCam;
}

App::~App()
{
    delete this->cubeMesh;
    delete this->TheMaze;
    delete this->ThePlayer;
    delete this->ObjCam;
    delete this->PlayerCam;
}

void App::initializeMaze(int mazeSize, int* mazeConfig)
{
  this->TheMaze->SetUpMaze(mazeSize, mazeSize, mazeConfig);
}

void App::SetShaders(int programID1, int programID2)
{
    this->programID1 = programID1;
    this->programID2 = programID2;
}

void App::render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(*ThePlayer);
    
    setupView(programID1);
    TheMaze->renderWalls(programID1);
    setupView(programID2);
    TheMaze->renderPlayer(programID2);
    TheMaze->renderGoal(programID2);
}

void App::MoveStraight(float moveAngle)
{
    float newPan = moveAngle + ThePlayer->GetPan();
    int index = (int)round(moveAngle / 90.f) % 4;
    glm::ivec2 vecArray[4] = {
        glm::ivec2(1,0),
        glm::ivec2(0,1),
        glm::ivec2(-1,0),
        glm::ivec2(0,-1)
    };
    glm::ivec2 moveVector = vecArray[index];
    glm::ivec2 newLocation = ThePlayer->GetLocation() + moveVector;
    if(TheMaze->IsLocationClear(newLocation))
        ThePlayer->SetLocation(newLocation);
    if(TheMaze->IsLocationGoal(newLocation))
        ThePlayer->SetLocation(newLocation);
}

void App::key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        switch(key) 
        {
            case GLFW_KEY_ESCAPE: // escape key pressed
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case '1':
                Camera = PlayerCam;
                setProjection();
                break;
            case '2':
                Camera = ObjCam;
                setProjection();
                break;
            case GLFW_KEY_UP:
                this->MoveStraight(180);
                break;
            case GLFW_KEY_DOWN:
                this->MoveStraight(0);
                break;
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
}   

void App::setupView(int progId) 
{
    // First load the viewing matrix from the current camera
    glm::mat4 viewMatrix;
    viewMatrix = Camera->getViewMtx();
    // Load it to the shader program
    int viewHandle = glGetUniformLocation(progId, "view");
    if (viewHandle == -1) {
        std::cout << "Uniform: view is not an active uniform label\n";
    }
    glUniformMatrix4fv( viewHandle, 1, false, glm::value_ptr(viewMatrix) );
}

void App::setProjection()
{
    float fov = (float) M_PI / 2.5f;
    glm::mat4 projection;
    projection = glm::perspective(fov, (float) winX / winY, 0.2f, 50.0f );

    // Load it to the shader program
    int projHandle = glGetUniformLocation(programID1, "projection");
    if (projHandle == -1) {
        std::cout << "Uniform: projection_matrix is not an active uniform label\n";
    }
    glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(projection) );
}    