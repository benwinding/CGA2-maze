#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>

// Include the graphics engine file
#include "engine.hpp"

#define PI 3.14159265358979323846

// A constant used describing the number of vertexs needed 
// for 1 side of a polygon with thickness
#define VERTICES_PER_SIDE 6

// Function definitions before implementations
float* MakeRingVertices(int sides, float radiusInner, float radiusOuter);
int getVertexCount(int sides);
void renderCallBack(double timeVal);
void sendVerticesToGPU(int n);

/* 
 * Program entry point
 */
int main(int argc, char **argv)
{ 
  int ndefault = 8;
  int n;
  // Parse program arguments
  if(argc > 1) {
    // Get first argument in argument array
    char* arg1 = argv[1];
    std::cout << "Argument 1, n_input=" << arg1 << '\n';
    try {
      // Try to parse the first arg as an integer
      n = std::stoi(arg1);
    } catch (std::exception const &e) {
      // If it failed set as default
      n = ndefault;
    }
  }
  else {
    // If no argument is provided, set default
    n = ndefault;
  }
  // Display argument that was set
  std::cout << "n=" << n << '\n';
  // Set shader paths
  engSetShaders("cube.vert", "cube.frag");
  // Set up graphics engine
  engSetup();
  // Create and send the vertices to engine
  sendVerticesToGPU(n);
  // Begin the render loop and pass the render callback into it
  engBeginRenderLoop(&renderCallBack);
}

/*
 Function for sending the vertices to the GPU before the render loop begins
*/
void sendVerticesToGPU(int n)
{
  // Cube has 8 vertices at its corners
  int CUBE_NUM_VERTICES = 8;
  int VALS_PER_VERT = 3;
  int verticesCount = CUBE_NUM_VERTICES * VALS_PER_VERT;
  float vertices[verticesCount] = {
    -1.0f, -1.0f, 1.0f ,
    1.0f, -1.0f, 1.0f ,
    1.0f,  1.0f, 1.0f ,
    -1.0f,  1.0f, 1.0f ,
    -1.0f, -1.0f, -1.0f ,
    1.0f, -1.0f, -1.0f ,
    1.0f,  1.0f, -1.0f ,
    -1.0f,  1.0f, -1.0f
  };
  // Each square face is made up of two triangles
  int CUBE_NUM_TRIS = 12;
  int indicesCount = CUBE_NUM_TRIS * 3;
  int indices[indicesCount] = {
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    5, 4, 7, 7, 6, 5,
    4, 0, 3, 3, 7, 4,
    3, 2, 6, 6, 7, 3,
    4, 5, 1, 1, 0, 4
  };
  engSetVertexData(vertices, verticesCount, indices, indicesCount);
}

/*
  A callback function, which gets called during the render loop
  Used to render the vertices on the GPU
*/
void renderCallBack(double timeVal) 
{
  float rotation = float(timeVal * 0.5);
  engDrawVerts(rotation*-0.3, 0, 0, 0, 1.0f, 1.0f);
  float offsetDist = 0.38f;
  float scaleFactor = 0.2f;
}
