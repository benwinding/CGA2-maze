#ifndef ENGINE_HPP
#define ENGINE_HPP

/*
The definitions for the RENDERING ENGINE
The rendering engine has the following responsibilities:
- Abstract the details of rendering
- Abstract opengl from the usecase-=
- Allow easier more readable code in the main part of the program
*/

// Define the Callback for render function
typedef void (*CallbackType)(double);
// Must be called first, sets shader programs
void engSetShaders(const char* shaderVertPath, const char* shaderFragPath);
// Sets a lot of the details of the rendering engine, only called once
void engSetup();
// Sets the vertex data to the GPU, called usually once
void engSetVertexData(float *vertices, int verticesCount, int *indices, int indicesCount);
// Begin the rendering loop and execute the callback function provided
void engBeginRenderLoop(CallbackType renderCallBack);
// Draw the vertices on the screen, which have been sent to the GPU already.
// Must be called from within the renderLoop callBack provided
void engDrawVerts(float rotZ1, float rotZ2, float transX, float transY, float scaleX, float scaleY);

#endif
