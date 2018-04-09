#ifndef SHADER_HPP
#define SHADER_HPP

/**************************************************
 * Simple function to read GLSL shader source from a file,
 * Then compile it and link to create a shader program ready for use.
 * Returns the ID of the shader program (assigned by OpenGL)
 * or 0 if error.
**************************************************/

#include <string>

GLuint LoadShaders(std::string vertex_file_path,
                   std::string fragment_file_path);

#endif
