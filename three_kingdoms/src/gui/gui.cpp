
#include "gui/gui.h"
#include "gui/core/gui_definition.h"
#include "preferences/preferences.h"
#include "gui/core/static_registry.h"
#include "gui/widgets/window.h"

namespace gui2
{
static bool initialized = false;

void init()
{
	if(initialized) {
		return;
	}
	std::cout << "Initializing UI subststem." << std::endl;
    resolution_definition  win_resol(800, 900, 800, 900, 800, 900, 800, 900);

    //[](const config& cfg) { return std::make_shared<type>(cfg); }, key); 
    register_widget("window", /*equal to get_control control_type*/
    [](const resolution_definition&win_resol) {  // 第二个参数：解析器函数
        return std::make_shared<window_definition>(win_resol);  // win_def 继承自 style_wid_def
    }, 
    nullptr);

    std::string id = "default";
    auto iter = guis.emplace(id, gui_definition(win_resol)).first;

    if(id == "default") {
        default_gui = iter;
        current_gui = default_gui;
    }
	// Save current screen size.
	//settings::update_screen_size_variables();

	//
	// Read and validate the WML files.
	//
	// config cfg;
	// try {
	// 	schema_validation::schema_validator validator(filesystem::get_wml_location("schema/gui.cfg"));

	// 	preproc_map preproc(game_config::config_cache::instance().get_preproc_map());
	// 	filesystem::scoped_istream stream = preprocess_file(filesystem::get_wml_location("gui/_main.cfg"), &preproc);

	// 	read(cfg, *stream, &validator);
	// } catch(const config::error& e) {
	// 	ERR_GUI_P << e.what();
	// 	ERR_GUI_P << "Setting: could not read file 'data/gui/_main.cfg'.";
	// } catch(const abstract_validator::error& e) {
	// 	ERR_GUI_P << "Setting: could not read file 'data/schema/gui.cfg'.";
	// 	ERR_GUI_P << e.message;
	// }

	//
	// Parse GUI definitions.
	//
	// const std::string& current_theme = prefs::get().gui_theme();

	// for(const config& g : cfg.child_range("gui")) {
	// 	const std::string id = g["id"];

	// 	auto iter = guis.emplace(id, gui_definition(g)).first;

	// 	if(id == "default") {
	// 		default_gui = iter;
	// 	}

	// 	if(!current_theme.empty() && id == current_theme) {
	// 		current_gui = iter;
	// 	}
	// }

	// VALIDATE(default_gui != guis.end(), _("No default gui defined."));

	// if(current_theme.empty()) {
	// 	current_gui = default_gui;
	// } else if(current_gui == guis.end()) {
	// 	ERR_GUI_P << "Missing [gui] definition for '" << current_theme << "'";
	// 	current_gui = default_gui;
	// }

	current_gui->second.activate();

	initialized = true;
}

} // namespace gui2
