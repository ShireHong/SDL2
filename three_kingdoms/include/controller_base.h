#pragma once

#include "events.h"


namespace events
{
class mouse_handler_base;
}

class controller_base : public events::sdl_handler
{
public:
    controller_base();
	virtual ~controller_base();

    virtual void play_slice(bool is_delay_enabled = true);


protected:
	/** Get a reference to a mouse handler member a derived class uses. */
	virtual events::mouse_handler_base& get_mouse_handler_base() = 0;

    void handle_event(const SDL_Event& event) override;

private:
    bool last_mouse_is_touch_;
	/** Context menu timer */
	size_t long_touch_timer_;
};