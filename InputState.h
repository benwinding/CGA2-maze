/**
 A set of variables that records mouse activity.
 You can update the variables yourself or use the functions.
*/

#ifndef _INPUTSTATE_H_
#define _INPUTSTATE_H_

struct InputState
{    
	InputState(): lMousePressed(false),
                  rMousePressed(false),
    	    	  prevX(0), prevY(0),
	    	      deltaX(0), deltaY(0) {}

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

    bool ReadKEY_UP()
    {
        bool res = KEY_UP;
        KEY_UP = false;
        return res;
    }

    bool ReadKEY_DOWN()
    {
        bool res = KEY_DOWN;
        KEY_DOWN = false;
        return res;
    }

    bool ReadKEY_LEFT()
    {
        bool res = KEY_LEFT;
        KEY_LEFT = false;
        return res;
    }

    bool ReadKEY_RIGHT()
    {
        bool res = KEY_RIGHT;
        KEY_RIGHT = false;
        return res;
    }




    // Update cursor variables based on new position x,y
    void update(float x, float y)
    {
        float xDiff = x - prevX;
        float yDiff = y - prevY;
        deltaX += xDiff;
        deltaY += yDiff;
        prevX = x;
        prevY = y;
    };

    // Read off the accumulated motion and reset it
    void readDeltaAndReset(float *x, float *y)
    {
        *x = deltaX;
        *y = deltaY;
        deltaX = 0;
        deltaY = 0;
    };
};

#endif // _INPUTSTATE_H_
