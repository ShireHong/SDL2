

#pragma once

//#include <optional>

#include <string>                       // for string
#include <vector>                       // for vector
#include "events.h"

class game_launcher
{
public:
    game_launcher(bool test);
	~game_launcher();

	bool init_video();
	void launch_game();
	

private:
	game_launcher(const game_launcher&) = delete;
	game_launcher& operator=(const game_launcher&) = delete;

    const events::event_context main_event_context_;

};

