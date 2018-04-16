#ifndef __APP_H_
#define __APP_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Maze.h"
#include "Player.h"
#include "Viewer.h"
#include "CubeMesh.h"
#include "Texture.h"

// Repsonsible Composing the program and 
class App {
private:
    CubeMesh *cubeMesh;

    Maze *TheMaze;
    Player *ThePlayer;

    Viewer *Camera;

    ObjectViewer *ObjCam;
    PlayerViewer *PlayerCam;

    Texture *TextureGround;
    Texture *TextureWalls;

    int winX, winY;

    int shaderID1, shaderID2;

    unsigned int programID1, programID2, programID3;
    void MoveStraight(float angle);
    bool TexturesOn;
    int CompileShader(std::string vertPath, std::string fragPath);  
    void SetShaders();
    void toggleTextures();
public:
    App(int winX, int winY, int mazeSize, int* mazeConfig);
    ~App();

    void render();

    void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mods);

    void playerMove(int key);
    void cameraChange(int camera);

    void setProjection(int progId);
    void setupView(int progId);
};

#endif
