#pragma once

#include "location.h"             // for map_location
#include "mouse_handler_base.h"       // for mouse_handler_base


#include <set>                          // for set
#include <vector>                       // for vector
#include <SDL2/SDL_events.h>                 // for SDL_MouseButtonEvent


class play_controller;

namespace events{

class mouse_handler : public mouse_handler_base {
public:
	mouse_handler(play_controller & pc);
	~mouse_handler();
	static mouse_handler* get_singleton() { return singleton_ ;}

protected:
	/**
	 * Use update to force an update of the mouse state.
	 */
	void mouse_motion(int x, int y, const bool browse, bool update=false, map_location loc = map_location::null_location()) override;

private:

	play_controller & pc_;

	// previous highlighted hexes
	// the hex of the selected unit and empty hex are "free"
	map_location previous_hex_;
	map_location previous_free_hex_;
	map_location selected_hex_;
	map_location next_unit_;
	/**
	 * If non-empty, current_paths_.destinations contains a cache of highlighted
	 * hexes, likely the movement range or attack range of a unit.
	 */
	bool unselected_paths_;
	bool unselected_reach_;
	int path_turns_;
	int side_num_;

	bool over_route_;
	bool reachmap_invalid_;
	bool show_partial_move_;

	static mouse_handler * singleton_;

	bool preventing_units_highlight_;
};

}