#include "InputState.h"

InputState::InputState(Maze *mazeRef) 
{
    maze = mazeRef;
    lMousePressed = false;
    rMousePressed = false;
    prevX = 0;
    prevY = 0;
    deltaX = 0;
    deltaY = 0;
}

bool InputState::ReadKEY_UP()
{
    bool res = KEY_UP;
    KEY_UP = false;
    return res;
}

bool InputState::ReadKEY_DOWN()
{
    bool res = KEY_DOWN;
    KEY_DOWN = false;
    return res;
}

bool InputState::ReadKEY_LEFT()
{
    bool res = KEY_LEFT;
    KEY_LEFT = false;
    return res;
}

bool InputState::ReadKEY_RIGHT()
{
    bool res = KEY_RIGHT;
    KEY_RIGHT = false;
    return res;
}

bool InputState::ReadKEY_A()
{
    bool res = KEY_A;
    KEY_A = false;
    return res;
}

bool InputState::ReadKEY_Z()
{
    bool res = KEY_Z;
    KEY_Z = false;
    return res;
}

bool InputState::ReadKEY_T()
{
    bool res = KEY_T;
    return res;
}

bool InputState::ReadKEY_C()
{
    bool res = KEY_C;
    return res;
}

// Update cursor variables based on new position x,y
void InputState::update(float x, float y)
{
    float xDiff = x - prevX;
    float yDiff = y - prevY;
    deltaX += xDiff;
    deltaY += yDiff;
    prevX = x;
    prevY = y;
};

// Read off the accumulated motion and reset it
void InputState::readDeltaAndReset(float *x, float *y)
{
    *x = deltaX;
    *y = deltaY;
    deltaX = 0;
    deltaY = 0;
};