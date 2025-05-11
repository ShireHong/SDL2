
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
resolution_definition::resolution_definition(const config& cfg)
{
	// DBG_GUI_P << "Parsing resolution " << window_width << ", " << window_height;

	// linked_groups = parse_linked_group_definitions(cfg);
}

resolution_definition::resolution_definition(const int win_w, 
                                            const int win_h,
                                            const int min_w,
                                            const int min_h,
                                            const int def_w,
                                            const int def_h,
                                            const int max_w,
                                            const int max_h)
	: window_width(win_w)
	, window_height(win_h)
	, min_width(min_w)
	, min_height(min_h)
	, default_width(def_w)
	, default_height(def_h)
	, max_width(max_w)
	, max_height(max_h)
{
	std::cout << "Parsing resolution " << window_width << ", " << window_height<<std::endl;

	//linked_groups = parse_linked_group_definitions(cfg);
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

styled_widget_definition::styled_widget_definition(const resolution_definition& resol_def)
	// : id(cfg["id"]), description(cfg["description"].t_str()), resolutions()
	: id("style"), resolutions()
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
