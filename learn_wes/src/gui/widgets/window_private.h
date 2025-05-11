
#pragma once

/**
 * @file
 * Helper for header for the window.
 *
 * @note This file should only be included by window.cpp.
 *
 * This file is being used for a small experiment similar like
 * gui/widgets/grid_private.hpp.
 */

#include "gui/widgets/window.h"

namespace gui2
{

/**
 * Helper to implement private functions without modifying the header.
 *
 * The class is a helper to avoid recompilation and only has static
 * functions.
 */
struct window_implementation
{
	/**
	 * Layouts the window.
	 *
	 * This part handles the actual layouting of the window.
	 *
	 * See @ref layout_algorithm for more information.
	 *
	 * @param window              The window to operate upon.
	 * @param maximum_width       The maximum width of the window.
	 * @param maximum_height      The maximum height of the window.
	 */
	static void layout(window& window,
					   const unsigned maximum_width,
					   const unsigned maximum_height);
};

} // namespace gui2
