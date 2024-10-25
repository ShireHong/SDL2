#ifndef INPUT_H
#define INPUT_H

#include <memory>
#include <string>
#include <SDL2/SDL.h>

///Enum for the 3 mouse buttons
enum MOUSE 
{ 
    LEFT = 1, 
    MIDDLE = 2, 
    RIGHT = 3 
};

///Input handler wrapper
/**
*  A simple wrapper around SDL's input handling system
*/
class Input
{
public:
    Input();
    ~Input();
    ///Initialize the input manager
    static void Init();
    ///Read event input
    static void PollEvent();
    //Functions for interacting with keyboard
    /**
    *  Check if a key is being pressed down
    *  @param keyCode The string of the key to check, ex: 'a'
    *  @return T if key is down
    */
    static bool KeyDown(std::string keyCode);
    /**
    *  Check if a key is being pressed down
    *  @param keyCode The SDL_SCANCODE of the key
    *  @return T if key is down
    */
    static bool KeyDown(int keyCode);
    //Functions for interacting with mouse
    /**
    *  Check if the mouse button is clicked
    *  @param button The button to check
    */
    static bool MouseClick(int button);
    /**
    *  Check if the mouse button is currently down
    *  @param button The button to check
    */
    static bool MouseDown(int button);
    ///Get the mouse button event
    static SDL_MouseButtonEvent GetClick();
    ///Check if the mouse moved
    static bool MouseMotion();
    ///Get the mouse motion
    static SDL_MouseMotionEvent GetMotion();
    ///Get the current mouse position
    static SDL_Point MousePos();
    
    static bool Quit();
    ///Clear input data, used when changing states to clear old input
    static void Clear();
    ///Close the joystick and any other input items that need to be exited
    static void Close();

private:
    static void ClearQuit();
    ///Clear the keystates
    static void ClearKeys();
    ///Clear the mouse
    static void ClearMouse();

private:
    static SDL_Event evt;
    static bool mQuit, mMouseMove, mMouseClick;
    static Uint8 *mKeyStates;
    static SDL_MouseButtonEvent mButtonEvt;
    static SDL_MouseMotionEvent mMotionEvt;
};

#endif