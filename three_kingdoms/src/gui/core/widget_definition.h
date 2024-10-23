
#pragma once

#include "config.h"
// #include "font/font_options.hpp"
// #include "font/text.hpp"
// #include "gui/core/linked_group_definition.hpp"
// #include "gui/auxiliary/typed_formula.hpp"

#include <vector>
#include <memory>
namespace gui2
{

/**
 * Contains the state info for a resolution.
 *
 * At the moment all states are the same so there is no need to use
 * inheritance. If that is needed at some point the containers should contain
 * pointers
 */
struct state_definition
{
	//explicit state_definition(const config& cfg);

	//config canvas_cfg_;
};

struct resolution_definition
{
	// explicit resolution_definition(unsigned int w, unsigned int h,unsigned int min_w, unsigned int min_h,unsigned int def_w, unsigned int def_h,unsigned int max_w, unsigned int max_h);
    explicit resolution_definition(const config& cfg);
	unsigned int window_width;
	unsigned int window_height;

	unsigned int min_width;
	unsigned int min_height;

	unsigned int default_width;
	unsigned int default_height;

	unsigned int max_width;
	unsigned int max_height;

	//std::vector<linked_group_definition> linked_groups;

	unsigned int text_extra_width;
	unsigned int text_extra_height;
	//typed_formula<unsigned> text_font_size;

	//font::family_class text_font_family;
	//font::pango_text::FONT_STYLE text_font_style;

	//std::vector<state_definition> state;
};

typedef std::shared_ptr<resolution_definition>
resolution_definition_ptr;

typedef std::shared_ptr<const resolution_definition>
resolution_definition_const_ptr;

struct styled_widget_definition
{
	explicit styled_widget_definition(const config& cfg);

	// template<class T>
	// void load_resolutions(const config& cfg)
	// {
	// 	for(const config& resolution : cfg.child_range("resolution")) {
	// 		resolutions.emplace_back(std::make_shared<T>(resolution));
	// 	}
	// }

	std::string id;
	//t_string description;

	std::vector<resolution_definition_ptr> resolutions;
};

typedef std::shared_ptr<styled_widget_definition> styled_widget_definition_ptr;

} // namespace gui2
