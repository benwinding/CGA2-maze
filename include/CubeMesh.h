#ifndef _MESH_H_
#define _MESH_H_

class CubeMesh
{
    unsigned int cubeVAOHandle;
    void createCubeVAO();
public:
    CubeMesh();
    void drawCube(int modelUniformHandle, 
        float tx, float ty, float tz, 
        float sx, float sy, float sz);    
    int getCubeVAOHandle();
};

#endif
