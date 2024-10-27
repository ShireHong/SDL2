
#include "gui/core/gui_definition.h"

#include "config.h"
// #include "formatter.hpp"
// #include "gui/core/log.hpp"
#include "gui/core/static_registry.h"
// #include "gui/widgets/settings.hpp"
// #include "wml_exception.hpp"

namespace gui2
{
gui_theme_map_t guis;
gui_theme_map_t::iterator current_gui = guis.end();
gui_theme_map_t::iterator default_gui = guis.end();

gui_definition::gui_definition()
	: widget_types()
	, window_types()
	, popup_show_delay_(0)
	, popup_show_time_(0)
	, help_show_time_(0)
	, double_click_time_(0)
	, repeat_button_repeat_time_(0)
	, sound_button_click_()
	, sound_toggle_button_click_()
	, sound_toggle_panel_click_()
	, sound_slider_adjust_()
{
}

const builder_window::window_resolution& get_window_builder(const std::string& type)
{
	// settings::update_screen_size_variables();

	const auto& current_windows = current_gui->second.window_types;
	const auto& default_windows = default_gui->second.window_types;

	auto iter = current_windows.find(type);

	if(iter == current_windows.end()) {
		// Current GUI is the default one and no window type was found. Throw.
		if(current_gui == default_gui) {
			throw window_builder_invalid_id();
		}

		// Else, try again to find the window, this time in the default GUI.
		iter = default_windows.find(type);

		if(iter == default_windows.end()) {
			throw window_builder_invalid_id();
		}
	}

	// const auto& resolutions = iter->second.resolutions;

	// VALIDATE(!resolutions.empty(), formatter() << "Window '" << type << "' has no resolutions.\n");

	// return get_best_resolution(resolutions, [&](const builder_window::window_resolution& res) {
	// 	return point(
	// 		static_cast<int>(res.window_width),
	// 		static_cast<int>(res.window_height)
	// 	);
	// });
}


} // namespace gui2
