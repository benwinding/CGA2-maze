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
    this->textureShader = CompileShader(prefix + "walls.vert", prefix + "walls.frag");
    this->plainShader = CompileShader(prefix + "plain.vert", prefix + "plain.frag");
    glUseProgram(this->textureShader);
}

void App::render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(*ThePlayer);
    
    if(this->TexturesOn)
    {
        setLightLocation(textureShader);
        setViewMtx(textureShader);
        setProjection(textureShader);
        TextureGround->Use();
        TheMaze->renderMazeBoundaries(textureShader);
        TextureWalls->Use();
        TheMaze->renderWalls(textureShader);        
        setViewMtx(plainShader);
        setProjection(plainShader);
        ThePlayer->renderPlayer(plainShader);
        TheMaze->renderGoal(plainShader);
    }
    else 
    {
        TextureGround->DontUse();
        TextureWalls->DontUse();
        setViewMtx(plainShader);
        setProjection(plainShader);
        TheMaze->renderMazeBoundaries(plainShader);
        TheMaze->renderWalls(plainShader);        
        setViewMtx(plainShader);
        setProjection(plainShader);
        ThePlayer->renderPlayer(plainShader);
        TheMaze->renderGoal(plainShader);
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

void App::setViewMtx(int progId) 
{
    glm::mat4 viewMatrix;
    viewMatrix = Camera->getViewMtx();
    int viewHandle = getUniformId(progId, "view");
    glUniformMatrix4fv( viewHandle, 1, false, glm::value_ptr(viewMatrix) );
}

void App::setProjection(int progId)
{
    float fov = (float) M_PI / 2.f;
    glm::mat4 projection;
    projection = glm::perspective(fov, (float) winX / winY, 0.2f, 10000.0f );
    int projHandle = getUniformId(progId, "projection");
    glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(projection) );
}    

void App::setLightLocation(int progId)
{
    int projHandle = getUniformId(progId, "lightPos");
    glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(glm::vec3(0,5,0)) );
}

int App::getUniformId(int progId, std::string uniformName)
{
    int handle = glGetUniformLocation(progId, uniformName.c_str());
    if (handle == -1) {
        std::cout << "Uniform: '" << uniformName << "' is not an active uniform label\n";
    }
    return handle;
}

int App::SetWindowSize(int x, int y)
{
    this->winX = x;
    this->winY = y;
}