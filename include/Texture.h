#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define NUM_TEXTURES 2

class Texture
{
	static int textureCount;
	static GLuint TexID[NUM_TEXTURES];
	int createTexture();
	int textureIndex;
	void SetTexture(std::string texturePath);

public:
	Texture(std::string texturePath);
	void Use();

};

#endif
