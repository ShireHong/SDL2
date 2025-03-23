
#include "gui/core/static_registry.h"

//#include "gui/core/log.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

namespace gui2
{
std::set<std::string>& registered_window_types()
{
	static std::set<std::string> result;
	return result;
}

void register_window(const std::string& id)
{
	bool added = false;
	std::tie(std::ignore, added) = registered_window_types().emplace(id);
	if(!added) {
		// WRN_GUI_P << "Window '" << id << "' already registered. Ignoring.";
        std::cout << "Window '" << id << "' already registered. Ignoring." << std::endl;
	}
}

std::map<std::string, registered_widget_parser>& registered_widget_types()
{
	static std::map<std::string, registered_widget_parser> result;
	return result;
}

void register_widget(const std::string& type, widget_parser_t f, const char* key)
{
	registered_widget_types()[type] = {f, key};
}

// std::map<std::string, widget_builder_func_t>& widget_builder_lookup()
// {
// 	static std::map<std::string, widget_builder_func_t> result;
// 	return result;
// }

// void register_widget_builder(const std::string& type, widget_builder_func_t functor)
// {
// 	widget_builder_lookup().emplace(type, functor);
// }

} // namespace gui2
