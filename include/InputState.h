/**
 A set of variables that records mouse activity.
 You can update the variables yourself or use the functions.
*/

#ifndef _INPUTSTATE_H_
#define _INPUTSTATE_H_

#include "Maze.h"

class InputState
{    
protected:
public:
	InputState(Maze *mazeRef);

    Maze *maze;

    // Is the mouse button currently being held down?
	bool lMousePressed;
    bool rMousePressed;

    // Last known position of the cursor
    float prevX;
	float prevY;

    // Accumulated change in cursor position. 
	float deltaX;
	float deltaY;

    bool KEY_UP;
    bool KEY_DOWN;
    bool KEY_LEFT;
    bool KEY_RIGHT;
    bool KEY_A;
    bool KEY_Z;
    bool KEY_T;
    bool KEY_C;

    bool ReadKEY_UP();
    bool ReadKEY_DOWN();
    bool ReadKEY_LEFT();
    bool ReadKEY_RIGHT();
    bool ReadKEY_A();
    bool ReadKEY_Z();
    bool ReadKEY_T();
    bool ReadKEY_C();

    void update(float x, float y);
    void readDeltaAndReset(float *x, float *y);
};

#endif
