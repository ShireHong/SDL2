
#include <iostream>
#include "gui/core/gui_definition.h"
#include "config.h"
// #include "formatter.hpp"
// #include "gui/core/log.hpp"
#include "gui/core/static_registry.h"
#include "gui/widgets/settings.h"
// #include "wml_exception.hpp"
#include <assert.h>

namespace gui2
{
gui_theme_map_t guis;
gui_theme_map_t::iterator current_gui = guis.end();
gui_theme_map_t::iterator default_gui = guis.end();

gui_definition::gui_definition(const resolution_definition& resol_def)
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
    , id_("gui definion")
{
    std::cout << "Parsing gui " << id_ << std::endl;
    std::cout << "register here" << std::endl;
    window_types.emplace("message", builder_window());
	//
	// Widget parsing
	//

	/** Parse widget definitions of each registered type. */
	for (auto it = registered_widget_types().begin(); it != registered_widget_types().end(); ++it) 
    {
        const auto& type_id = it->first;
        const auto& widget_parser = it->second;
		auto& def_map = widget_types[type_id];

		const std::string key =	widget_parser.key
			? widget_parser.key
			: type_id + "_definition";

		bool found_default_def = false;

        // Run the static parser to get a definition ptr.
        styled_widget_definition_ptr def_ptr = widget_parser.parser(resol_def);

        const std::string& def_id = def_ptr->id;

        if(def_map.find(def_id) != def_map.end()) {
            // ERR_GUI_P << "Skipping duplicate definition '" << def_id << "' for '" << type_id << "'";
            std::cout<<"Skipping duplicate definition '" << def_id << "' for '" << type_id << "'"<<std::endl;
            continue;
        }

        def_map.emplace(def_id, std::move(def_ptr));

        if(def_id == "default") {
            found_default_def = true;
        }
		

		// Only the default GUI needs to ensure each widget has a default definition.
		// Non-default ones can just fall back to the default definition in the default GUI.
		if(id_ == "default") 
        {
			// VALIDATE(found_default_def, "No default definition found for widget '" + type_id + "'");
            if (!found_default_def)
            {
                std::cout<<"No default definition found for widget '" + type_id + "'"<<std::endl;
            }
		}
	}
}


void gui_definition::activate() const
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
        // std::cout<<"default_gui"<<std::endl;
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

namespace
{
template<typename TList, typename TConv>
const typename TList::value_type& get_best_resolution(const TList& list, const TConv& get_size)
{
	using resolution_t = const typename TList::value_type;

	resolution_t* best_resolution = nullptr;
	int best_resolution_score = std::numeric_limits<int>::min();

	const int screen_w = settings::screen_width;
	const int screen_h = settings::screen_height;

	for(const auto& res : list) {
		point size = get_size(res);

		int w = size.x ? size.x : 1;
		int h = size.y ? size.y : 1;
		int score = 0;

		if(w <= screen_w && h <= screen_h) {
			score = w * h;
		} else {
			// Negative score, only used in case none of the given resolution fits on the screen
			// (workaround for a bug where the windows size can become < 800x600).
			score = std::min(screen_w - w, 0) + std::min(screen_h - h, 0);
		}

		if(score >= best_resolution_score) {
			best_resolution = &res;
			best_resolution_score = score;
		}
	}

	assert(best_resolution != nullptr);
	return *best_resolution;
}

}

resolution_definition_ptr get_control(const std::string& control_type, const std::string& definition)
{
	const auto& current_types = current_gui->second.widget_types;
	const auto& default_types = default_gui->second.widget_types;

	const auto widget_definitions = current_types.find(control_type);

	gui_definition::widget_definition_map_t::const_iterator control;

	if(widget_definitions == current_types.end()) {
		goto fallback;
	}

	control = widget_definitions->second.find(definition);

	if(control == widget_definitions->second.end()) {
	fallback:
		bool found_fallback = false;

		if(current_gui != default_gui) {
			auto default_widget_definitions = default_types.find(control_type);

			// VALIDATE(widget_definitions != current_types.end(),
			// 	formatter() << "Type '" << control_type << "' is unknown.");
            if (widget_definitions == current_types.end())
            {
                std::cout<< "Type '" << control_type << "' is unknown."<<std::endl;
            }

			control = default_widget_definitions->second.find(definition);
			found_fallback = control != default_widget_definitions->second.end();
		}

		if(!found_fallback) 
        {
			if(definition != "default") 
            {
				// LOG_GUI_G << "Control: type '" << control_type << "' definition '" << definition
				// 		  << "' not found, falling back to 'default'.";
                std::cout << "Control: type '" << control_type << "' definition '" << definition
						  << "' not found, falling back to 'default'."<<std::endl;
				return get_control(control_type, "default");
			}

			// FAIL(formatter() << "default definition not found for styled_widget " << control_type);
            std::cout<< "default definition not found for styled_widget " << control_type<<std::endl;
		}
	}

	const auto& resolutions = (*control->second).resolutions;

	// VALIDATE(!resolutions.empty(),
	// 	formatter() << "Control: type '" << control_type << "' definition '" << definition << "' has no resolutions.\n");
    if (resolutions.empty())
    {
        std::cout<< "Control: type '" << control_type << "' definition '" << definition << "' has no resolutions.\n"<<std::endl;
    }

	return get_best_resolution(resolutions, [&](const resolution_definition_ptr& ptr) {
		return point(
			static_cast<int>(ptr->window_width),
			static_cast<int>(ptr->window_height)
		);
	});
}

} // namespace gui2
