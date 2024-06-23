#include "mouse_handler_base.h"
#include "sdl/input.h"




namespace events
{
    mouse_handler_base::mouse_handler_base()
    {
    }

    void mouse_handler_base::mouse_motion_event(const SDL_MouseMotionEvent& event, const bool browse)
    {
        mouse_motion(event.x, event.y, browse);
    }

    void mouse_handler_base::mouse_update(const bool browse, map_location loc)
    {
        int x, y;
        sdl::get_mouse_state(&x, &y);
        mouse_motion(x, y, browse, true, loc);
    }
}

