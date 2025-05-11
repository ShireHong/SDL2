
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
    resolution_definition  win_resol(300, 200, 0, 0, 300, 200, 600, 400);

    //[](const config& cfg) { return std::make_shared<type>(cfg); }, key); 
    register_widget("window", /*get_control  control_type)*/
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

	current_gui->second.activate();

	initialized = true;
}

} // namespace gui2
