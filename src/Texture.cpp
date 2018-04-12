#include "Texture.h"

#define TEX_SIZE 64  // resolution of our constructed texture image

#define VALS_PER_VERT 3
#define VALS_PER_TEX 2          // number of dimensions of the texture data (if image, set it to 2)
#define POLYGON_NUM_TRIS 2      // number of triangles in our mesh
#define POLYGON_NUM_VERTICES 4     // number of vertices in our mesh

int Texture::textureCount = 0;

Texture::Texture(std::string texturePath)
{
	this->textureIndex = textureCount;
	textureCount++;
}

void Texture::SetTexture(std::string texturePath, int shaderProgramId)
{
	// unsigned int buffer[3];
 //    // Texture coords for each vertex. 2 per vertex.
 //    float tex_coord[POLYGON_NUM_VERTICES * VALS_PER_TEX] = {
 //        0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
 //    };

	// int texLoc = glGetAttribLocation(shaderProgramId, "a_tex_coord");

	// // Texture attributes
	// glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
	// glEnableVertexAttribArray(texLoc);
	// glVertexAttribPointer(texLoc, VALS_PER_TEX, GL_FLOAT, GL_FALSE, 0, 0);
}

int Texture::createTexture()
{
    // unsigned int buffer[3];
    // glGenBuffers(3, buffer);

    // glActiveTexture(GL_TEXTURE0);

    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // glGenTextures( NUM_TEXTURES, this->TexID );

    // // generate mipmaps for the texture 
    // glGenerateMipmap(GL_TEXTURE_2D); 
        
    // // load an image from file as texture 1
    // int x, y, n;
    // unsigned char *data = stbi_load(
    //                                 "crate.jpg", /*char* filepath */
    //                                 &x, The address to store the width of the image
    //                                 &y, /*The address to store the height of the image*/
    //                                 &n  /*Number of channels in the image*/,
    //                                 0   /*Force number of channels if > 0*/
    // );

    // glBindTexture( GL_TEXTURE_2D, this->TexID[this->textureIndex] );
    // if (n == 3) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // }
    // else {
    //     fprintf(stderr, "Image pixels are not RGB. Texture may not load correctly.");
    // }
    // stbi_image_free(data);

    // glGenerateMipmap(GL_TEXTURE_2D); 

    return 0;
}


void Texture::Use()
{
	
}