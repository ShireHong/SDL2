#pragma once


#include <SDL2/SDL_events.h>
#include "location.h"
namespace events
{

extern int commands_disabled;

class mouse_handler_base
{
public:
    mouse_handler_base();

	virtual ~mouse_handler_base()
	{
	}

    void mouse_motion_event(const SDL_MouseMotionEvent& event, const bool browse);

	//void touch_motion_event(const SDL_TouchFingerEvent& event, const bool browse);

    void mouse_update(const bool browse, map_location loc);

    virtual void mouse_motion(
			int x, int y, const bool browse, bool update = false, map_location new_loc = map_location::null_location())
			= 0;
    
    // virtual void mouse_press(const SDL_MouseButtonEvent& event, const bool browse);
	// virtual bool mouse_button_event(const SDL_MouseButtonEvent& event, uint8_t button, map_location loc, bool click = false);
};
}