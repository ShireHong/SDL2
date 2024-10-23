
#include <iostream>
#include "gui/core/widget_definition.h"

// #include "gettext.hpp"
// #include "gui/core/log.hpp"
// #include "gui/widgets/helper.hpp"
// #include "wml_exception.hpp"

namespace gui2
{

// state_definition::state_definition(const config& cfg)
// 	: canvas_cfg_(VALIDATE_WML_CHILD(cfg, "draw", _("No draw section defined for state.")))
// {}

// resolution_definition::resolution_definition(unsigned int w, unsigned int h,unsigned int min_w, unsigned int min_h,unsigned int def_w, unsigned int def_h,unsigned int max_w, unsigned int max_h)
// 	: window_width(w)
// 	, window_height(w)
// 	, min_width(min_w)
// 	, min_height(min_h)
// 	, default_width(def_w)
// 	, default_height(def_h)
// 	, max_width(max_w)
// 	, max_height(max_h)
// {
// 	std::cout << "Parsing resolution " << window_width << ", " << window_height<<std::endl;

// 	//linked_groups = parse_linked_group_definitions(cfg);
// }
resolution_definition::resolution_definition(const config& cfg)
{
	// DBG_GUI_P << "Parsing resolution " << window_width << ", " << window_height;

	// linked_groups = parse_linked_group_definitions(cfg);
}

styled_widget_definition::styled_widget_definition(const config& cfg)
	// : id(cfg["id"]), description(cfg["description"].t_str()), resolutions()
{
	// VALIDATE(!id.empty(), missing_mandatory_wml_key("styled_widget", "id"));
	// VALIDATE(!description.empty(),
	// 		 missing_mandatory_wml_key("styled_widget", "description"));

	// /*
	//  * Do this validation here instead of in load_resolutions so the
	//  * translatable string is not in the header and we don't need to pull in
	//  * extra header dependencies.
	//  */
	// config::const_child_itors itors = cfg.child_range("resolution");
	// VALIDATE(!itors.empty(), _("No resolution defined."));
}

} // namespace gui2
