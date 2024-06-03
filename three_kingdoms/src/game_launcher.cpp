#include <stdexcept>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h> // for SDL_Init, SDL_INIT_TIMER
#include "game_launcher.h"
#include "play_controller.h"

game_launcher::game_launcher(bool test)
{
    std::cout<<"game_launcher"<<std::endl;
}


bool game_launcher::init_video()
{
	// Initialize video subsystem, and create a new window.
	//video::init();

	// Set window title and icon
	//video::set_window_title(game_config::get_default_title_string());

// #if !(defined(__APPLE__))
// 	surface icon(image::get_surface(image::locator{"icons/icon-game.png"}, image::UNSCALED));
// 	if(icon != nullptr) {
// 		video::set_window_icon(icon);
// 	}
// #endif
	return true;
}

void game_launcher::launch_game()
{
	play_controller playcontroller;
    playcontroller.play_slice_catch();

}


game_launcher::~game_launcher()
{
}
