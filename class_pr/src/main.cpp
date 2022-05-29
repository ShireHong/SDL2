#include <stdexcept>
#include <memory>
#include "window.h"
#include "statemanager.h"

#undef main
int main(int argc, char** argv)
{
    
    Window::Init("class_ptr", 640, 480);
    
    //Input::Init();

    StateManager::InitIntro();

    //Input::Close();
    //Debug::Quit();
    Window::Quit();

    return 0;
}