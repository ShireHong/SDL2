#include <stdlib.h>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h> // for SDL_Init, SDL_INIT_TIMER
#include <time.h>
#include <cstdlib>
#include "gui/gui.h"                  // for init
#include "game_launcher.h"
#include "events.h"
#include "gui/dialogs/message.h"      // for show_error_message
#undef main



/**
 * I would prefer to setup locale first so that early error
 * messages can get localized, but we need the game_launcher
 * initialized to have filesystem::get_intl_dir() to work.  Note: setlocale()
 * does not take GUI language setting into account.
 */
static void init_locale()
{
#if defined _WIN32 || defined __APPLE__
	setlocale(LC_ALL, "English");
#else
	std::setlocale(LC_ALL, "C");
#endif
}

/**
 * Setups the game environment and enters
 * the titlescreen or game loops.
 */
static int do_gameloop()
{
	srand(time(nullptr));

	const auto game = std::make_unique<game_launcher>(true);
	const int start_ticks = SDL_GetTicks();

	init_locale();

    bool res;

	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);


    res = game->init_video();
	if(res == false) {
		std::cout << "could not initialize display" <<std::endl;
		return 1;
	}

    gui2::init();
    
    if (true)
    {
        std::string msg ="Unable to create ";
		gui2::show_message("Logging Failure", msg, gui2::dialogs::message::ok_button);
    }

    // If loading a game, skip the titlescreen entirely
    //if(game->has_load_data() && game->load_game()) {
    game->launch_game();
        //continue;
    //}

}


int main(int argc, char** argv)
{

#ifdef _WIN32
	putenv("PANGOCAIRO_BACKEND=fontconfig");
	putenv("FONTCONFIG_PATH=fonts");
#endif

    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
	// Is there a reason not to just use SDL_INIT_EVERYTHING?
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		std::cout << "Couldn't initialize SDL: " << SDL_GetError();
		return (1);
	}
	atexit(SDL_Quit);

    SDL_StartTextInput();
    
    //Window::Init("Three_kingdoms", 202, 200);
    //Input::Init();

    events::event_context global_context;

    do_gameloop();
    //StateManager::InitIntro();
    // Input::Close();
    // //Debug::Quit();
    // Window::Quit();

    return 0;
}