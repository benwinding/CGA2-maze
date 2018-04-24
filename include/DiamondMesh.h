#ifndef _DMESH_H_
#define _DMESH_H_

class DiamondMesh
{
    unsigned int diamondVAOHandle;
    void createDiamondVAO();
    glm::mat4 model;
    int shaderID;
    int scaleUniformHandle;
    glm::vec2 textureScale;

    int getDiamondVAOHandle();

public:
    DiamondMesh();

    void Reset(int shaderId);
    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float rx);
    void RotateY(float ry);
    void RotateZ(float ry);
    void SetTextureScale(float s, float t);
    void Draw();

    void MakeDiamond(int shaderId, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz);
};

#endif
