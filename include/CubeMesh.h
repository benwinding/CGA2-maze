#ifndef _MESH_H_
#define _MESH_H_

class CubeMesh
{
    unsigned int cubeVAOHandle;
    void createCubeVAO();
    glm::mat4 model;
    int shaderID;
    int scaleUniformHandle;
    glm::vec2 textureScale;

    int getCubeVAOHandle();

public:
    CubeMesh();

    void Reset(int shaderId);
    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float rx);
    void RotateY(float ry);
    void RotateZ(float ry);
    void SetTextureScale(float s, float t);
    void Draw();

    void MakeCube(int shaderId, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz);
};

#endif
