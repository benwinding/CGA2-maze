#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NUM_TEXTURES 2

class Texture
{
	int handle;
	int createTexture();
    int makeTextureHandle(std::string texturePath);

public:
	Texture(std::string texturePath);
	void Use();
	void DontUse();

};

#endif
