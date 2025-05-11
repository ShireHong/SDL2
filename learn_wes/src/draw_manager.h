
#pragma once

#include "sdl/rect.h"

namespace gui2 { class top_level_drawable; }


namespace draw_manager
{


void invalidate_region(const rect& region);

/** Mark the entire screen as requiring redraw. */
void invalidate_all();


void request_extra_render_pass();


void sparkle();

int get_frame_length();


void register_drawable(gui2::top_level_drawable* tld);

/** Remove a top-level drawable from the drawing stack. */
void deregister_drawable(gui2::top_level_drawable* tld);

/** Raise a TLD to the top of the drawing stack. */
void raise_drawable(gui2::top_level_drawable* tld);

} // namespace draw_manager
