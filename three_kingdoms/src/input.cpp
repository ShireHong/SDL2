#include <memory>
#include <string>
#include <SDL.h>
#include "window.h"
#include "math.h"
#include "input.h"

SDL_Event Input::evt;
Uint8* Input::mKeyStates = nullptr;
SDL_MouseButtonEvent Input::mButtonEvt;
SDL_MouseMotionEvent Input::mMotionEvt;
bool Input::mQuit = false;
bool Input::mMouseMove = false;
bool Input::mMouseClick = false;

Input::Input()
{

}
Input::~Input()
{

}
void Input::Init()
{
    mKeyStates = (Uint8*)SDL_GetKeyboardState(NULL);
}

void Input::PollEvent()
{
    //Clear mouse data
    ClearMouse();

    //Read the event stack
    while(SDL_PollEvent(&evt)){
        //this is an ok method to do this. But maybe not the best
        //Window::HandleEvents(evt);

        if (evt.type == SDL_QUIT)
            mQuit = true;
        //Send mouse click events
        if ((evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP)){
            mMouseClick = true;
            mButtonEvt = evt.button;
        }
        //Send mouse movement events
        if (evt.type == SDL_MOUSEMOTION){
            mMouseMove = true;
            mMotionEvt = evt.motion;
        }
    }
}

bool Input::KeyDown(std::string keyCode)
{
    switch (keyCode.at(0)){
        case '1':
	        return (mKeyStates[SDL_SCANCODE_1] == 1);
        case '2':
	        return (mKeyStates[SDL_SCANCODE_2] == 1);
        case '3':
	        return (mKeyStates[SDL_SCANCODE_3] == 1);
        case '4':
	        return (mKeyStates[SDL_SCANCODE_4] == 1);
        case '5':
	        return (mKeyStates[SDL_SCANCODE_5] == 1);
        case '6':
	        return (mKeyStates[SDL_SCANCODE_6] == 1);
        case '7':
	        return (mKeyStates[SDL_SCANCODE_7] == 1);
        case '8':
	        return (mKeyStates[SDL_SCANCODE_8] == 1);
        case '9':
	        return (mKeyStates[SDL_SCANCODE_9] == 1);
        case '0':
	        return (mKeyStates[SDL_SCANCODE_0] == 1);
        case 'q':
	        return (mKeyStates[SDL_SCANCODE_Q] == 1);
        case 'w':
	        return (mKeyStates[SDL_SCANCODE_W] == 1);
        case 'e':
	        return (mKeyStates[SDL_SCANCODE_E] == 1);
        case 'r':
	        return (mKeyStates[SDL_SCANCODE_R] == 1);
        case 't':
	        return (mKeyStates[SDL_SCANCODE_T] == 1);
        case 'y':
	        return (mKeyStates[SDL_SCANCODE_Y] == 1);
        case 'u':
	        return (mKeyStates[SDL_SCANCODE_U] == 1);
        case 'i':
	        return (mKeyStates[SDL_SCANCODE_I] == 1);
        case 'o': 
	        return (mKeyStates[SDL_SCANCODE_O] == 1);
        case 'p':
	        return (mKeyStates[SDL_SCANCODE_P] == 1);
        case 'a':
	        return (mKeyStates[SDL_SCANCODE_A] == 1);
        case 's':
	        return (mKeyStates[SDL_SCANCODE_S] == 1);
        case 'd':
	        return (mKeyStates[SDL_SCANCODE_D] == 1);
        case 'f':
	        return (mKeyStates[SDL_SCANCODE_F] == 1);
        case 'g':
	        return (mKeyStates[SDL_SCANCODE_G] == 1);
        case 'h':
	        return (mKeyStates[SDL_SCANCODE_H] == 1);
        case 'j':
	        return (mKeyStates[SDL_SCANCODE_J] == 1);
        case 'k':
	        return (mKeyStates[SDL_SCANCODE_K] == 1);
        case 'l':
	        return (mKeyStates[SDL_SCANCODE_L] == 1);
        case 'z':
	        return (mKeyStates[SDL_SCANCODE_Z] == 1);
        case 'x':
	        return (mKeyStates[SDL_SCANCODE_X] == 1);
        case 'c':
	        return (mKeyStates[SDL_SCANCODE_C] == 1);
        case 'v':
	        return (mKeyStates[SDL_SCANCODE_V] == 1);
        case 'b':
	        return (mKeyStates[SDL_SCANCODE_B] == 1);
        case 'n':
	        return (mKeyStates[SDL_SCANCODE_N] == 1);
        case 'm':
	        return (mKeyStates[SDL_SCANCODE_M] == 1);
        default:
	        return false;
    }
}
bool Input::KeyDown(int keyCode)
{
    //Keep the keyCode in range
    if (keyCode > 282)
        return false;
    return (mKeyStates[keyCode] == 1);
}

bool Input::MouseClick(int button)
{
    return (mMouseClick && mButtonEvt.button == button);
}

bool Input::MouseDown(int button)
{
    return ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button)) != 0);
}

SDL_MouseButtonEvent Input::GetClick()
{
    return mButtonEvt;
}

bool Input::MouseMotion()
{
    //We can filter out the first event, which has false motion data by testing
    //if x == xrel and y = yrel, which only happens when starting the program
    return (mMouseMove && (mMotionEvt.x != mMotionEvt.xrel 
	    && mMotionEvt.y != mMotionEvt.yrel));
}

SDL_MouseMotionEvent Input::GetMotion(){
    return mMotionEvt;
}

SDL_Point Input::MousePos()
{
    SDL_Point pos;
    pos.x = 0;
    pos.y = 0;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

bool Input::Quit(){
    return mQuit;
}

void Input::Clear(){
    ClearQuit();
    ClearKeys();
    ClearMouse();
}

void Input::Close()
{
    
}

void Input::ClearQuit(){
    mQuit = false;
}
void Input::ClearKeys(){
    //I'm not sure if there's a better way to 0 out the values
    //for now this will do
    for (int i = 0; i < 282; ++i)
	    mKeyStates[i] = 0;
}

void Input::ClearMouse()
{
    mMouseClick = false;
    mMouseMove = false;
}
