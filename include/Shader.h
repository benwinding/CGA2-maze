#ifndef _SHADER_H_
#define _SHADER_H_

/**************************************************
 * Simple function to read GLSL shader source from a file,
 * Then compile it and link to create a shader program ready for use.
 * Returns the ID of the shader program (assigned by OpenGL)
 * or 0 if error.
**************************************************/

#include <string>
#include <glm/glm.hpp>

class Shader
{
	int shaderId;
	int getHandle(std::string uniformName);
	int CompileShader(const char *ShaderPath, const uint ShaderID);
	uint LoadShaders(std::string vertex_file_path, std::string fragment_file_path);
public:
	Shader(std::string vertex_file_path, std::string fragment_file_path);
	void setVec3(std::string uniformName, const glm::vec3 &vec);
	void setVec3(std::string uniformName, float x, float y, float z);
	void setRgb(std::string uniformName, int x, int y, int z);
	void setMat4(std::string uniformName, const glm::mat4 &mat);
	int GetId();
	void use();
};



#endif
