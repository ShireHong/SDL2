#include <stdexcept>
#include <memory>
#include <iostream>
#include "window.h"
#include "statemanager.h"
#include "input.h"

#undef main
int main(int argc, char** argv)
{
    
    Window::Init("Three_kingdoms", 640, 480);
    
    Input::Init();

    StateManager::InitIntro();
    std::cout<<"exit"<<std::endl;
    Input::Close();
    //Debug::Quit();
    Window::Quit();

    return 0;
}