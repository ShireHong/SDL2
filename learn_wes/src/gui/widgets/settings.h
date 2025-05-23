
#pragma once

// #include "gui/auxiliary/tips.hpp"
// #include "tstring.hpp"

#include <string>
#include <vector>

namespace gui2
{


/** Do we wish to use the new library or not. */
extern bool new_widgets;

/** This namespace contains the 'global' settings. */
namespace settings
{
/**
 * The screen resolution and pixel pitch should be available for all widgets since
 * their drawing method might depend on it.
 */
extern unsigned screen_width;
extern unsigned screen_height;
extern const unsigned screen_pitch_microns; /* Deprecated, do not use */

/**
 * The offset between the left edge of the screen and the gamemap.
 */
extern unsigned gamemap_x_offset;

/**
 * The size of the map area, if not available equal to the screen
 * size.
 */
extern unsigned gamemap_width;
extern unsigned gamemap_height;

/** These are copied from the active gui. */
extern unsigned popup_show_delay;
extern unsigned popup_show_time;
extern unsigned help_show_time;
extern unsigned double_click_time;
extern unsigned repeat_button_repeat_time;

extern std::string sound_button_click;
extern std::string sound_toggle_button_click;
extern std::string sound_toggle_panel_click;
extern std::string sound_slider_adjust;

// extern t_string has_helptip_message;

// extern std::vector<game_tip> tips;

/**
 * Update the size of the screen variables in settings.
 *
 * Before a window gets build the screen sizes need to be updated. This
 * function does that. It's only done when no other window is active, if
 * another window is active it already updates the sizes with it's resize
 * event.
 */
void update_screen_size_variables();
}

} // namespace gui2
