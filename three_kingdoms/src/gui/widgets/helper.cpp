
#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/helper.h"

#include "color.h"
// #include "formula/callable.hpp"
// #include "formula/string_utils.hpp"
// #include "gui/core/log.hpp"
#include "sdl/point.h"
#include "gui/widgets/settings.h"
// #include "tstring.hpp"
#include "sdl/input.h" // get_mouse_location


namespace gui2
{
// font::pango_text::FONT_STYLE decode_font_style(const std::string& style)
// {
// 	static const std::map<std::string, font::pango_text::FONT_STYLE> font_style_map {
// 		{"normal",    font::pango_text::STYLE_NORMAL},
// 		{"bold",      font::pango_text::STYLE_BOLD},
// 		{"italic",    font::pango_text::STYLE_ITALIC},
// 		{"underline", font::pango_text::STYLE_UNDERLINE},
// 	};

// 	if(style.empty()) {
// 		return font::pango_text::STYLE_NORMAL;
// 	}

// 	if(const auto i = font_style_map.find(style); i != font_style_map.end()) {
// 		return i->second;
// 	}

// 	ERR_GUI_G << "Unknown style '" << style << "', using 'normal' instead.";
// 	return font::pango_text::STYLE_NORMAL;
// }

// color_t decode_color(const std::string& color)
// {
// 	return color_t::from_rgba_string(color);
// }

// PangoWeight decode_text_weight(const std::string& weight)
// {
// 	if(weight == "thin") {
// 		return PANGO_WEIGHT_THIN;
// 	} else if (weight == "light") {
// 		return PANGO_WEIGHT_LIGHT;
// 	} else if (weight == "semibold") {
// 		return PANGO_WEIGHT_SEMIBOLD;
// 	} else if (weight == "bold") {
// 		return PANGO_WEIGHT_BOLD;
// 	} else if (weight == "heavy") {
// 		return PANGO_WEIGHT_HEAVY;
// 	}

// 	if(!weight.empty() && weight != "normal") {
// 		ERR_GUI_E << "Invalid text weight '" << weight << "', falling back to 'normal'.";
// 	}

// 	return PANGO_WEIGHT_NORMAL;
// }

// PangoStyle decode_text_style(const std::string& style)
// {
// 	if(style == "italic") {
// 		return PANGO_STYLE_ITALIC;
// 	} else if(style == "oblique") {
// 		return PANGO_STYLE_OBLIQUE;
// 	}

// 	if(!style.empty() && style != "normal") {
// 		ERR_GUI_E << "Invalid text style '" << style << "', falling back to 'normal'.";
// 	}

// 	return PANGO_STYLE_NORMAL;
// }

// PangoAlignment decode_text_alignment(const std::string& alignment)
// {
// 	if(alignment == "center") {
// 		return PANGO_ALIGN_CENTER;
// 	} else if(alignment == "right") {
// 		return PANGO_ALIGN_RIGHT;
// 	}

// 	if(!alignment.empty() && alignment != "left") {
// 		ERR_GUI_E << "Invalid text alignment '" << alignment << "', falling back to 'left'.";
// 	}

// 	return PANGO_ALIGN_LEFT;
// }

// std::string encode_text_alignment(const PangoAlignment alignment)
// {
// 	switch(alignment) {
// 		case PANGO_ALIGN_LEFT:
// 			return "left";
// 		case PANGO_ALIGN_RIGHT:
// 			return "right";
// 		case PANGO_ALIGN_CENTER:
// 			return "center";
// 	}

// 	assert(false);
// 	// FIXME: without this "styled_widget reaches end of non-void function" in release mode
// 	throw "Control should not reach this point.";
// }

// t_string missing_widget(const std::string& id)
// {
// 	return t_string(VGETTEXT("Mandatory widget '$id' hasn't been defined.", {{"id", id}}));
// }

// void get_screen_size_variables(wfl::map_formula_callable& variable)
// {
// 	variable.add("screen_width", wfl::variant(settings::screen_width));
// 	variable.add("screen_height", wfl::variant(settings::screen_height));
// 	variable.add("screen_pitch_microns", wfl::variant(settings::screen_pitch_microns));
// 	variable.add("gamemap_width", wfl::variant(settings::gamemap_width));
// 	variable.add("gamemap_height", wfl::variant(settings::gamemap_height));
// 	variable.add("gamemap_x_offset", wfl::variant(settings::gamemap_x_offset));
// }

// wfl::map_formula_callable get_screen_size_variables()
// {
// 	wfl::map_formula_callable result;
// 	get_screen_size_variables(result);

// 	return result;
// }

point get_mouse_position()
{
	return sdl::get_mouse_location();
}

// std::string_view debug_truncate(std::string_view text)
// {
// 	return text.substr(0, 15);
// }

} // namespace gui2
