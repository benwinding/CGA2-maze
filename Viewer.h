
/**
 Some viewer classes representing cameras that fixate on an object.
 All viewers inherit from a base Viewer class.
 See model-view.cpp for example usage.
 Anthony Dick
*/

#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "InputState.h"
#include "glm/glm.hpp"

/**
 The base Viewer class stores a current view transformation.
 It is initialised by providing an initial camera position.
 Assumes we are always looking at the origin.
 It is updated in response to user input or it can be reset.
 Method orthogonaliseViewMtx() can be used to correct drift if needed.
*/
class Viewer 
{
protected:
    glm::mat4 viewMtx;
    glm::vec3 initEye;
    glm::vec3 initAt;
    glm::vec3 initUp;

public:
    Viewer( glm::vec3 eye );
    virtual ~Viewer() = 0;

    const glm::mat4 getViewMtx() const;
    //    void orthogonaliseViewMtx();
    void reset();

    virtual void update( InputState &input ) = 0;
};


/**
 ObjectViewer rotates about current camera x and y axes in
 response to mouse motion. To do this it reads the current 
 camera axes from the viewing matrix.
*/
class ObjectViewer : public Viewer
{
public:
    ObjectViewer( glm::vec3 eye );

    virtual void update( InputState &input );
};

/**
 PlayerViewer rotates from the keyboard. To do this it 
 reads the current camera axes from the viewing matrix.
*/
class PlayerViewer : public Viewer
{
private: 
    glm::vec3 currentEyeX;
    float currentTilt;
    float currentPan;
    void RotatePan(float rotY);
    void RotateTilt(float rotX);
    void TranslateStraight(float deltaMove);

public:
    PlayerViewer( glm::vec3 eye );

    virtual void update( InputState &input );
};

#endif // VIEWER_H
