#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>

#include "glm/gtx/string_cast.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "App.h"
#include "Shader.hpp"

App::App(int winX, int winY, int mazeSize, int* mazeConfig)
{
    this->winX = winX;
    this->winY = winY;
    this->cubeMesh = new CubeMesh();
    this->ThePlayer = new Player(this->cubeMesh);
    this->TheMaze = new Maze(this->cubeMesh, this->ThePlayer);
    
    this->TheMaze->SetUpMaze(mazeSize, mazeSize, mazeConfig);

    this->ObjCam = new ObjectViewer(glm::vec3(0,mazeSize*2,0.5));
    this->PlayerCam = new PlayerViewer(glm::vec3(0,0,0), this->TheMaze->GetMazeSize());
    this->Camera = PlayerCam;

    this->TextureGround = new Texture("res/star_wars_trench.png");
    this->TextureWalls = new Texture("res/crate.png");
    this->TexturesOn = true;

    // setupView(programID1);
    // setProjection(programID1);
    // TextureGround->Use();
    // TextureWalls->Use();

    this->SetShaders();
}

App::~App()
{
    delete this->cubeMesh;
    delete this->TheMaze;
    delete this->ThePlayer;
    delete this->ObjCam;
    delete this->PlayerCam;
}

int App::CompileShader(std::string vertPath, std::string fragPath)
{
    int shaderId = LoadShaders(vertPath, fragPath);
    if(shaderId == 0)
    {
        std::cout << "Failed to compile: " << vertPath << " and " << fragPath << std::endl; 
    }
    return shaderId;
}

void App::SetShaders()
{
    std::string prefix = "res/shaders/";
    // Set up the shaders we are to use. 0 indicates error.
    this->programID1 = CompileShader(prefix + "walls.vert", prefix + "walls.frag");
    this->programID2 = CompileShader(prefix + "walls.vert", prefix + "walls.frag");
    this->programID3 = CompileShader(prefix + "plain.vert", prefix + "plain.frag");
    glUseProgram(this->programID1);
}

void App::render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(*ThePlayer);
    
    if(this->TexturesOn)
    {
        setupView(programID1);
        setProjection(programID1);
        TextureGround->Use();
        TheMaze->renderMazeBoundaries(programID1);
        TextureWalls->Use();
        TheMaze->renderWalls(programID1);        
        setupView(programID2);
        setProjection(programID2);
        TheMaze->renderPlayer(programID2);
        TheMaze->renderGoal(programID2);
    }
    else 
    {
        TextureGround->DontUse();
        TextureWalls->DontUse();
        setupView(programID3);
        setProjection(programID3);
        TheMaze->renderMazeBoundaries(programID3);
        TheMaze->renderWalls(programID3);        
        setupView(programID3);
        setProjection(programID3);
        TheMaze->renderPlayer(programID3);
        TheMaze->renderGoal(programID3);
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
            case GLFW_KEY_ESCAPE: // escape key pressed
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

void App::setProjection(int progId)
{
    float fov = (float) M_PI / 2.f;
    glm::mat4 projection;
    projection = glm::perspective(fov, (float) winX / winY, 0.2f, 10000.0f );

    // Load it to the shader program
    int projHandle = glGetUniformLocation(progId, "projection");
    if (projHandle == -1) {
        std::cout << "Uniform: projection_matrix is not an active uniform label\n";
    }
    glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(projection) );
}    
