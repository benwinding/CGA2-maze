
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "Viewer.h"
#include "glm/gtc/matrix_transform.hpp"

#define DEG2RAD(x) ((x)*M_PI/180.0) 
#define RAD2DEG(x) ((x)*180.0/M_PI) 

Viewer::Viewer( glm::vec3 eye )
{
    initEye = eye;
}

Viewer::~Viewer(){}

/**
 Assumes the current matrix has been calculated (usually in 
 update() or reset())
*/
const glm::mat4 Viewer::getViewMtx() const
{
    return viewMtx;
}

/**
 Resets the view matrix to the value the camera was 
 initialised with. Assumes looking at the origin.
*/
void Viewer::reset()
{
    glm::vec3 at(0.0f, 1.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(initEye, at, up);
}

ObjectViewer::ObjectViewer(glm::vec3 eye)
    : Viewer(eye)
{
    reset();
}

void ObjectViewer::update( InputState &input ) 
{
    float xRot, yRot;
    input.readDeltaAndReset( &yRot, &xRot );
    
    if ( input.lMousePressed )
    {
        // The first 3 rows of the view matrix are the camera x, y, z axes
        // in world coordinate space. (see lecture 6)
        glm::vec3 eyeX( viewMtx[0][0], viewMtx[1][0], viewMtx[2][0] );
        glm::vec3 eyeY( viewMtx[0][1], viewMtx[1][1], viewMtx[2][1] );
        glm::vec3 eyeZ( viewMtx[0][2], viewMtx[1][2], viewMtx[2][2] );

        // Rotate about the eye's y axis.
        if ( yRot != 0 )
        {
            float sinY = sin(DEG2RAD(yRot));
            float cosY = cos(DEG2RAD(yRot));

            glm::vec3 tmpX = eyeX;
            eyeX = cosY*tmpX + sinY*eyeZ;
            eyeZ = -sinY*tmpX + cosY*eyeZ;
        }
        // Rotate about the eye's x axis.
        if ( xRot != 0 )
        {
            float sinX = sin(DEG2RAD(xRot));
            float cosX = cos(DEG2RAD(xRot));

            glm::vec3 tmpY = eyeY;
            eyeY = cosX*tmpY - sinX*eyeZ;
            eyeZ = sinX*tmpY + cosX*eyeZ;
        }

        // Update the view matrix with new eye axes.
        viewMtx[0][0] = eyeX[0];
        viewMtx[1][0] = eyeX[1];
        viewMtx[2][0] = eyeX[2];
        
        viewMtx[0][1] = eyeY[0];
        viewMtx[1][1] = eyeY[1];
        viewMtx[2][1] = eyeY[2];
        
        viewMtx[0][2] = eyeZ[0];
        viewMtx[1][2] = eyeZ[1];
        viewMtx[2][2] = eyeZ[2];
    }
}

// Player Viewer
PlayerViewer::PlayerViewer(glm::vec3 eye)
    : Viewer(eye)
{
    reset();
}

void PlayerViewer::RotatePan(float deltaTurn)
{
    float deltaRotY = DEG2RAD(deltaTurn);
    viewMtx = glm::translate(viewMtx, initEye);
    viewMtx = glm::rotate(viewMtx, deltaRotY, glm::vec3(0, 1.0f, 0));
    viewMtx = glm::translate(viewMtx, -initEye);
    currentPan = currentPan + deltaRotY;
}

void PlayerViewer::RotateTilt(float deltaTurn)
{
    float deltaRotX = DEG2RAD(deltaTurn);
    glm::vec3 eyeX(viewMtx[0][0], viewMtx[1][0], viewMtx[2][0]);
    viewMtx = glm::translate(viewMtx, initEye);
    viewMtx = glm::rotate(viewMtx, deltaRotX, eyeX);
    viewMtx = glm::translate(viewMtx, -initEye);
    currentTilt = currentTilt + deltaTurn;
}

void PlayerViewer::TranslateStraight(float deltaMove)
{
    float tilt = currentTilt;
    RotateTilt(-tilt);
    glm::vec3 eyeZ(viewMtx[0][2], viewMtx[1][2], viewMtx[2][2]);
    glm::vec3 normZ = glm::normalize(eyeZ);
    glm::vec3 zScaled = normZ * deltaMove;
    viewMtx = glm::translate(viewMtx, zScaled);
    initEye = initEye - zScaled;
    RotateTilt(tilt);
}

bool PlayerViewer::MazeInteferes( InputState &input ) 
{
    if (input.ReadKEY_C())
        return false;

    float x = initEye[0] - input.maze->GetWidth() / 2;
    float z = initEye[2] - input.maze->GetWidth() / 2;
    int xr = round(x);
    int zr = round(z);

    return input.maze->IsCollision(xr, zr);
}

void PlayerViewer::SetPlayerPosition(InputState &input)
{
    float x = (initEye[0] + 0.5f);
    float z = (initEye[2] + 0.5f);
    std::cout << "Viewer: Player coordinates: x=" << x << ",z=" << z << std::endl;
    glm::vec3 temp(x, 0, z);
    input.maze->SetPosition(temp);
}

void PlayerViewer::update( InputState &input ) 
{
    float deltaMove = .2f;
    float deltaTurn = 10.f;

    if ( input.ReadKEY_UP() ) {
        if (MazeInteferes(input)) {
            std::cout << "Viewer: Cannot Move Interference" << std::endl;
            return;
        }
        TranslateStraight(deltaMove);
        SetPlayerPosition(input);
    }
    else if ( input.ReadKEY_DOWN() ) {
        if (MazeInteferes(input)) {
            std::cout << "Viewer: Cannot Move Interference" << std::endl;
            return;
        }
        TranslateStraight(-deltaMove);
        SetPlayerPosition(input);
    }
    else if ( input.ReadKEY_LEFT() ) {
        RotatePan(-deltaTurn);
    }
    else if ( input.ReadKEY_RIGHT() ) {
        RotatePan(deltaTurn);
    }
    else if ( input.ReadKEY_A() ) {
        RotateTilt(-deltaTurn);
    }
    else if ( input.ReadKEY_Z() ) {
        RotateTilt(deltaTurn);
    }
}
