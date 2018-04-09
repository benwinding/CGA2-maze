#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "App.h"
#include "Shader.hpp"

int winX = 500;
int winY = 500;

App* TheApp;

// Called when the window is resized.
void reshape_callback( GLFWwindow *window, int x, int y ) 
{
    winX = x;
    winY = y;
    TheApp->setProjection();
    glViewport( 0, 0, x, y );
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

/**
 * Parse program arguments, read maze file, create maze object
 */
int ParseAndReadMazeFile(int argc, char **argv)
{
    std::ifstream infile;
    int mazeSize;
    int* mazeLayout;
    // Parse program arguments
    if(argc > 1) {
        // Get first argument in argument array
        char* arg1 = argv[1];
        std::cout << "Program input: " << arg1 << '\n';
        try {
            // Open file
            infile.open(arg1);
            // Detect maze size
            std::string mazeSizeString; 
            infile >> mazeSizeString;
            mazeSize = std::stoi(mazeSizeString);
            std::cout << "Read Maze Size: " << mazeSize << std::endl;
            // Initialize maze layout array
            mazeLayout = new int[mazeSize*mazeSize];
            // Assign values to maze layout
            std::cout << "Read Maze Configuration: " << std::endl;
            char c;
            int i = 0;
            int j = 0;
            while (infile.get(c)) 
            {
                int index = i*mazeSize+j;
                if(c == '\n') 
                    continue;
                else if(c == ' ') 
                    mazeLayout[index] = 0;
                else if(c == '*') 
                    mazeLayout[index] = 1;
                else if(c == 'X') 
                    mazeLayout[index] = 2;
                // std::cout << "(" << i << "," << j << ")=" << c << std::endl;
                std::cout << c;
                if(i == mazeSize-1) {
                    j++;
                    std::cout << std::endl;
                }
                i = (i + 1) % (mazeSize);
            }
            std::cout << std::endl;
        } catch (std::exception const &e) {
            // If bad argument is provided, exit
            std::cout << "Bad filename recieved, exitting..." << std::endl;
            return 0;
        }
    }
    else {
        // If no argument is provided, exit
        std::cout << "No filename recieved, exitting..." << std::endl;
        return 0;
    }

    TheApp->initializeMaze(mazeSize, mazeLayout);
    return 1;
}

void PrintHelp() 
{
    const char * helpScreen = R"V0G0N(
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ~~~ Welcome 2 Assignment 2! ~~~
        ~~~~~~~ By Ben Winding ~~~~~~~~

    Usage: 
        ./assign2 FILENAME

    Keyboard:
        UP    = Move Forward
        DOWN  = Move Backward
        LEFT  = Turn Left
        RIGHT = Turn Right

        A     = Tilt Up
        Z     = Tilt Down

        ESC   = Exit Program

        T     = Textures on/off
        C     = Collisions on/off

        1 = First Person View (Mouse disabled)
        2 = World View (Mouse enabled)

    )V0G0N";
    std::cout << helpScreen << std::endl;
}

void key_callback(GLFWwindow* window,
    int key, int scancode, int action, int mods)
{
    TheApp->key_callback(window, key, scancode, action, mods);
}   

int main (int argc, char **argv)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        exit(1);
    }

    // Specify that we want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and OpenGL context
    window = glfwCreateWindow(winX, winY, "Modelling and viewing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
	
  	// Initialize GLEW
  	glewExperimental = true; // Needed for core profile
  	if (glewInit() != GLEW_OK) 
    {
    	fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
  	}

    std::string prefix = "res/shaders/";
    // Set up the shaders we are to use. 0 indicates error.
    int programID1 = LoadShaders(prefix + "mazewalls.vert", prefix + "mazewalls.frag");
    if (programID1 == 0) 
    {
        exit(1);
    }
  	// Set up the shaders we are to use. 0 indicates error.
  	int programID2 = LoadShaders(prefix + "mazegoal.vert", prefix + "mazegoal.frag");
  	if (programID2 == 0) 
    {
    	exit(1);
    }
    TheApp->SetShaders(programID1, programID2);

    // Set OpenGL state we need for this application.
    glUseProgram(programID1);
    glClearColor(0.5F, 0.5F, 0.5F, 0.0F);
    glEnable(GL_DEPTH_TEST);
    
    // Print program help
    PrintHelp();

    // Parse program arguments and read maze
    if (!ParseAndReadMazeFile(argc, argv)) 
    {
        exit(1);
    }
    
    // Define callback functions and start main loop
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, reshape_callback);

    TheApp->setProjection();

    bool firstRender = true;
    while (!glfwWindowShouldClose(window))
    {
        TheApp->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(firstRender)
            TheApp->setProjection();
        firstRender = false;
    }

    // Clean up
    delete TheApp;
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
    
    return 0;
}
