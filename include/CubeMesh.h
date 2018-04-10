#ifndef _MESH_H_
#define _MESH_H_

class CubeMesh
{
    unsigned int cubeVAOHandle;
    void createCubeVAO();
    glm::mat4 model;
public:
    CubeMesh();

	void Reset();
	void Translate(float tx, float ty, float tz);
	void Scale(float sx, float sy, float sz);
	void RotateX(float rx);
	void RotateY(float ry);
	void RotateZ(float ry);
	void Draw(int modelUniformHandle);

	void drawCube(int modelUniformHandle, 
    float tx, float ty, float tz, 
    float sx, float sy, float sz);

    int getCubeVAOHandle();
};

#endif
