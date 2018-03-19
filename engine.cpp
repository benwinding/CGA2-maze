/**
 * file:   engine.cpp
 * author: Ben Winding
 * 
 * This file is a "graphics engine" and is designed to make the 
 * main.cpp easier to read.
 * 
 * Responsibilities of engine.cpp include: 
 *  - Simplify access to the GLFW and openGL + GLSL api's
 *  - Absract implementation of graphics from program.
 *  - Separate concerns of what is being drawn and how it's drawn
 *  - Improve readability and resusability of the program.
 */

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// GLEW loads OpenGL extensions. Required for all OpenGL programs.
#include <GL/glew.h>
// GLFW is a cross-platform windowing API, see glfw.org
#include <GLFW/glfw3.h>
// GLM is a matrix library for OpenGL and GLSL applications
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Utility code to load and compile GLSL shader programs
#include "shader.hpp"

// The values given per vertice
#define VALS_PER_VERT 3

// A variable to hold the number of Vertices
static int NUM_VERTS;
// Handle to our VAO generated in setShaderData method
unsigned int vertexVaoHandle; 
// Handle to our shader program
unsigned int programID;
// Reference to window
GLFWwindow* window;
// Define the Callback for render function
typedef void (*CallbackType)(double);
// References to the shader filepaths
const char* shaderVert;
const char* shaderFrag;

// Declare functions before using them.
void renderSetup();
void render(double dt);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
/**
 * Sets the shader uniforms and vertex data
 * This happens ONCE only, before any frames are rendered
 * @returns 0 for success, error otherwise
 */

void engSetVertexData(float *vertices, int verticesCount, int *indices, int indicesCount)
{
  /*
   * What we want to draw
   * Each set of 3 vertices (9 floats) defines one triangle
   * You can define more triangles to draw here
   */

  NUM_VERTS = verticesCount / VALS_PER_VERT / 4;
    
  // Generate storage on the GPU for our triangle and make it current.
  // A VAO is a set of data buffers on the GPU
  glGenVertexArrays(1, &vertexVaoHandle);   
  glBindVertexArray(vertexVaoHandle);

  // Generate new buffers in our VAO
  // A single data buffer store for per-vertex attributes (e.g. position)
  unsigned int buffer[2];
  glGenBuffers(2, buffer);

  // Allocate GPU memory for our vertices and copy them over
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, verticesCount, vertices, GL_STATIC_DRAW);

  // Allocate GPU memory for our vertices and copy them over
  glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ARRAY_BUFFER, indicesCount, indices, GL_STATIC_DRAW);
  
  // Now we tell OpenGL how to interpret the data we just gave it
  // Tell OpenGL what shader variable it corresponds to
  // Tell OpenGL how it's formatted (floating point, 3 values per vertex)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

  // An argument of zero un-binds all VAO's and stops us
  // from accidentally changing the VAO state
  glBindVertexArray(0);

  // The same is true for buffers, so we un-bind it too
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Error callback for GLFW. Simply prints error message to stderr.
 */
static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

// Setting the shader paths, called once
void engSetShaders(const char* shaderVertPath, const char* shaderFragPath) {
  shaderVert = shaderVertPath;
  shaderFrag = shaderFragPath;
}

// Set up the graphics engine, called once 
void engSetup() 
{
  glfwSetErrorCallback(error_callback);
  
  if (!glfwInit())
    exit(1);

  // Specify that we want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create the window and OpenGL context
  window = glfwCreateWindow(480, 480, "Computer Graphics - Assignment 1", NULL, NULL);
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
    
  // Sets the (background) colour for each time the frame-buffer
  // (colour buffer) is cleared
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Set up the shaders we are to use. 0 indicates error.
  programID = LoadShaders(shaderVert, shaderFrag);
  if (programID == 0)
    exit(1);
    
  // To draw a surface with outlined polygons, call
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Add a callback on keyboard press
  glfwSetKeyCallback(window, key_callback);
}

/**
 * Program entry. Sets up OpenGL state, GLSL Shaders and GLFW window and function call backs
 * Takes no arguments
 */
void engBeginRenderLoop(CallbackType renderCallBack)
{
  // Reference to time, so animation can be accomplished
  // Start time
  double start = glfwGetTime();
  // Current time
  double now;

  // The event loop, runs until the window is closed.
  // Each iteration redraws the window contents and checks for new events.
  // Windows are double buffered, so need to swap buffers.
  // GLFW records the time since the program started (in seconds)
  while (!glfwWindowShouldClose(window))
  {
    // Set current time in loop
    now = glfwGetTime();
    // Setup boiler plate tasks in the render loop
    renderSetup();
    // Access the given callback function and pass the current time to it.
    renderCallBack(now - start);

    glBindVertexArray(0); // Un-bind the VAO
    glFlush();  // Guarantees previous commands have been completed before continuing
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // On exit Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(0);
}

/**
 * Renders a frame of the state and shaders we have set up to the window
 * Executed each time a frame is to be drawn.
 */
int mtxHandle; // Hodl reference to the transform in th shader
bool wireFrame = false;
void renderSetup() 
{
  // Clear the previous pixels we have drawn to the colour buffer (display buffer)
  // Called each frame so we don't draw over the top of everything previous
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Specify the shader program we want to use
  glUseProgram(programID);

  // Make the VAO with our vertex data buffer current
  glBindVertexArray(vertexVaoHandle);

  // Get a reference to the transform in the shader, so we can update it
  mtxHandle = glGetUniformLocation(programID, "transform");
  if (mtxHandle == -1)
    exit(1);
}

// Gets the current draw mode, used to switch between wireframe and filled
int getDrawMode()
{
  // Change from either wireframe or filled
  if(wireFrame) {
    return GL_LINE_LOOP;
  } 
  else {
    return GL_TRIANGLES;
  }
}

void engDrawVerts(float rotZ1, float rotZ2, float transX, float transY, float scaleX, float scaleY)
{
  glm::mat4 transMatrix;
  glUniformMatrix4fv(mtxHandle, 1, false, glm::value_ptr(transMatrix) );

  transMatrix = glm::rotate(transMatrix, rotZ1, glm::vec3(0.0f, 0.0f, 1.0f));
  transMatrix = glm::translate(transMatrix, glm::vec3(transX, transY, 1.0f));
  transMatrix = glm::scale(transMatrix, glm::vec3(scaleX, scaleY, 1.0f));
  transMatrix = glm::rotate(transMatrix, rotZ2, glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv( mtxHandle, 1, false, glm::value_ptr(transMatrix) );
  glDrawArrays(getDrawMode(), 0, NUM_VERTS);
}

// Called on keypress, handles different keypresses
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    wireFrame = !wireFrame;
  }
  else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}
