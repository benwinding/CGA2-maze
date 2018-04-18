#ifndef __APP_H_
#define __APP_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Maze.h"
#include "Player.h"
#include "Viewer.h"
#include "CubeMesh.h"
#include "Texture.h"

// Repsonsible Composing the program and 
class App {
private:
    int winX, winY;

    CubeMesh *cubeMesh;

    Maze *TheMaze;
    Player *ThePlayer;

    Viewer *Camera;

    ObjectViewer *ObjCam;
    PlayerViewer *PlayerCam;

    bool TexturesOn;
    Texture *TextureGround;
    Texture *TextureWalls;

    Shader *textureShader;
    Shader *plainShader;

    glm::mat4 projection;


    int CompileShader(std::string vertPath, std::string fragPath);  
    void MoveStraight(float angle);
    void SetShaders();
    void toggleTextures();

    int getUniformId(int progId, std::string uniformName);
    void setProjection(int progId);
    void setViewMtx(int progId);
    void setLightLocation(int progId);
    void updateProjection();

public:
    App(int winX, int winY, int mazeSize, int* mazeConfig);
    ~App();

    void render();

    void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mods);

    int SetWindowSize(int x, int y);
};

#endif
