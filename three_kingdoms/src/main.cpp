#include <stdexcept>
#include <memory>
#include <iostream>
#include "window.h"
#include "statemanager.h"
#include "input.h"

#undef main
int main(int argc, char** argv)
{
    
<<<<<<< HEAD
    Window::Init("Three_kingdoms", 960, 720);
=======
    Window::Init("Three_kingdoms", 640, 480);
>>>>>>> 96f14378fc7d1e52705c499877fd97229aaff2f0
    
    Input::Init();

    StateManager::InitIntro();
    std::cout<<"exit"<<std::endl;
    Input::Close();
    //Debug::Quit();
    Window::Quit();

    return 0;
}