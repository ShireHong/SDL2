#include <stdexcept>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h> // for SDL_Init, SDL_INIT_TIMER
#include <SDL_image.h>
#include "game_launcher.h"
#include "play_controller.h"
#include "video.h"

game_launcher::game_launcher(bool test)
              : main_event_context_()
{
    std::cout<<"game_launcher"<<std::endl;
}


bool game_launcher::init_video()
{
	video::init();

	// Set window title and icon
	video::set_window_title("Three kingdoms");

#if !(defined(__APPLE__))
	// surface icon(image::get_surface(image::locator{"icons/icon-game.png"}, image::UNSCALED));
	// if(icon != nullptr) {
	// 	video::set_window_icon(icon);
	// }
    SDL_Surface* icon = IMG_Load("icons/icon-game.png");
    if (icon  != nullptr) 
    {
        //SDL_SetWindowIcon(video::window_, icon);
        //video::set_window_icon(icon);
    }
#endif
	return true;
}

void game_launcher::launch_game()
{
    
    play_controller playcontroller;
    while(1)
    {

        playcontroller.play_slice_catch();
    }

}


game_launcher::~game_launcher()
{
}
