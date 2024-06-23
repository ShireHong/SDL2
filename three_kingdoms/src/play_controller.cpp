#include <iostream>
#include "play_controller.h"
#include "resources.h"

static void clear_resources()
{
	resources::controller = nullptr;
}

play_controller::play_controller()
                : mouse_handler_(*this)
{
    //std::cout<<"play_controller"<<std::endl;
    resources::controller = this;
}

play_controller::~play_controller()
{
    clear_resources();
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