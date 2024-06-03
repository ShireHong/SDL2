#include <cassert>     // for assert
#include <new>         // for bad_alloc
#include <ostream>     // for operator<<, basic_ostream, etc
#include <string>      // for string, operator<<, etc

#include "mouse_events.h"
#include "play_controller.h"
#include "input_ws.h"



namespace events
{
mouse_handler::mouse_handler(play_controller& pc)
	: mouse_handler_base()
	, pc_(pc)
	, previous_hex_()
	, previous_free_hex_()
	, selected_hex_()
	, next_unit_()
	, unselected_paths_(false)
	, unselected_reach_(false)
	, path_turns_(0)
	, side_num_(1)
	, over_route_(false)
	, reachmap_invalid_(false)
	, show_partial_move_(false)
	, preventing_units_highlight_(false)
{
	singleton_ = this;
}

mouse_handler::~mouse_handler()
{
	singleton_ = nullptr;
}

void mouse_handler::mouse_motion(int x, int y, const bool browse, bool update, map_location new_hex)
{
	// we ignore the position coming from event handler
	// because it's always a little obsolete and we don't need
	// to highlight all the hexes where the mouse passed.
	// Also, sometimes it seems to have one *very* obsolete
	// and isolated mouse motion event when using drag&drop
	sdl::get_mouse_state(&x, &y); // <-- modify x and y

	
}

mouse_handler* mouse_handler::singleton_ = nullptr;

}