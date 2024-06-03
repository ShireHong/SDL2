#include <iostream>
#include "play_controller.h"



play_controller::play_controller()
                : mouse_handler_(*this)
{
    std::cout<<"play_controller"<<std::endl;
}

play_controller::~play_controller()
{

}


void play_controller::play_slice_catch()
{
	// if(should_return_to_play_side()) {
	// 	return;
	// }

    play_slice();
	
}

events::mouse_handler& play_controller::get_mouse_handler_base()
{
	return mouse_handler_;
}