/**
 Scene built from cubes
 Camera is swappable:
 '1' - camera rotates about camera axes
 '2' - camera rotates about world axes
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "InputState.h"
#include "Viewer.h"
#include "Maze.h"
#include "Shader.hpp"

/**
 Variables controlling how the scene is drawn
*/

Maze *TheMaze;

int winX = 500;
int winY = 500;

/**
 Cameras
*/
Viewer *Camera;
ObjectViewer *ObjCam;
PlayerViewer *PlayerCam;

glm::vec3 cameraPos(0.0f, 3.0f, -15.0f);
glm::vec3 playerPos(3.0f, 1.0f, 0.0f);

// Data structure storing mouse input info
InputState Input;

unsigned int programID1, programID2;

// Set the projection matrix. Takes into account window aspect ratio, so called
// when the window is resized.
void setProjection()
{
    float fov;
    if(Camera == PlayerCam)
        fov = (float) M_PI / 2.5f;
    else
        fov = (float) M_PI / 3.0f;

    glm::mat4 projection;
    projection = glm::perspective(fov, (float) winX / winY, 0.2f, 50.0f );

	// Load it to the shader program
	int projHandle = glGetUniformLocation(programID1, "projection");
	if (projHandle == -1) {
		std::cout << "Uniform: projection_matrix is not an active uniform label\n";
	}
	glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(projection) );
}    

//
// Callbacks
//
void key_callback(GLFWwindow* window,
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
                Input.KEY_UP = true;
                break;
            case GLFW_KEY_DOWN:
                Input.KEY_DOWN = true;
                break;
            case GLFW_KEY_LEFT:
                Input.KEY_LEFT = true;
                break;
            case GLFW_KEY_RIGHT:
                Input.KEY_RIGHT = true;
                break;
            case GLFW_KEY_A:
                Input.KEY_A = true;
                break;
            case GLFW_KEY_Z:
                Input.KEY_Z = true;
                break;
            default:
                Input.KEY_UP = false;
                Input.KEY_DOWN = false;
                Input.KEY_LEFT = false;
                Input.KEY_RIGHT = false;
                break;
        }
    }
}   

// Called when the window is resized.
void reshape_callback( GLFWwindow *window, int x, int y ) 
{
    winX = x;
    winY = y;
    setProjection();
    glViewport( 0, 0, x, y );
}

void mouse_pos_callback(GLFWwindow* window, double x, double y)
{
    // Use a global data structure to store mouse info
    // We can then use it however we want
    Input.update((float)x, (float)y);
}    

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(Camera == PlayerCam)
        return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Input.rMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Input.rMousePressed = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Input.lMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        Input.lMousePressed = false;
    }                
}

void setupView(int progId) 
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

void render() 
{
    // Update the camera, and draw the scene.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update( Input );    
    
    setupView(programID1);
    TheMaze->renderWalls(programID1);
    setupView(programID2);
    TheMaze->renderGoal(programID2);
}

/**
 * Error callback for GLFW. Simply prints error message to stderr.
 */
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

    TheMaze = new Maze(mazeSize, mazeSize, mazeLayout);
    TheMaze->SetPosition(0,0,90);
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
        ESC   = Exit Program

        1 = First Person View (Mouse disabled)
        2 = World View (Mouse enabled)

    )V0G0N";
    std::cout << helpScreen << std::endl;
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
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
	}

    // Set up the shaders we are to use. 0 indicates error.
    programID1 = LoadShaders("mazewalls.vert", "mazewalls.frag");
    if (programID1 == 0) {
        exit(1);
    }
	// Set up the shaders we are to use. 0 indicates error.
	programID2 = LoadShaders("mazegoal.vert", "mazegoal.frag");
	if (programID2 == 0) {
		exit(1);
    }

    // Set OpenGL state we need for this application.
    glUseProgram(programID1);
    glClearColor(0.5F, 0.5F, 0.5F, 0.0F);
    glEnable(GL_DEPTH_TEST);
    
    // Print program help
    PrintHelp();

    // Parse program arguments and read maze
    if (!ParseAndReadMazeFile(argc, argv)) {
        exit(1);
    }
    
    // Set cameras, with locations
    ObjCam = new ObjectViewer( cameraPos );
    PlayerCam = new PlayerViewer( playerPos );
    Camera = PlayerCam;


    // Define callback functions and start main loop
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, reshape_callback);

    setProjection();

    bool firstRender;
    while (!glfwWindowShouldClose(window))
    {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(!firstRender)
            setProjection();
    }

    // Clean up
    delete TheMaze;
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
    
    return 0;
}
