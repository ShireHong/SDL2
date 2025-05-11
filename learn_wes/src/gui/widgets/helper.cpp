#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/helper.h"

#include "color.h"
// #include "formula/callable.hpp"
// #include "formula/string_utils.hpp"
// #include "gui/core/log.hpp"
#include "sdl/point.h"
#include "gui/widgets/settings.h"
#include "sdl/rect.h"
// #include "tstring.hpp"
#include "sdl/input.h" // get_mouse_location

#include <SDL2/SDL.h>

namespace gui2
{

point get_mouse_position()
{
	return sdl::get_mouse_location();
}

std::string_view debug_truncate(std::string_view text)
{
	return text.substr(0, 15);
}

} // namespace gui2
