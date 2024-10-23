
#include "gui/core/top_level_drawable.h"

//#include "draw_manager.hpp"

namespace gui2
{

top_level_drawable::top_level_drawable()
{
	//draw_manager::register_drawable(this);
}

top_level_drawable::~top_level_drawable()
{
	//draw_manager::deregister_drawable(this);
}

top_level_drawable::top_level_drawable(const top_level_drawable&)
{
	//draw_manager::register_drawable(this);
}

top_level_drawable& top_level_drawable::operator=(const top_level_drawable&)
{
	//draw_manager::register_drawable(this);
	return *this;
}

top_level_drawable::top_level_drawable(top_level_drawable&&)
{
	//draw_manager::register_drawable(this);
}

top_level_drawable& top_level_drawable::operator=(top_level_drawable&&)
{
	//draw_manager::register_drawable(this);
	return *this;
}

} // namespace gui2
