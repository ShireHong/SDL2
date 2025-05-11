
#pragma once

#include <string>
#include "config.h"

//Similar to config_of but it stores references to configs (instead of cyoping them).
struct configr_of
{
	template <typename AT>
	configr_of(const std::string& attrname, AT value) : subtags_(), data_()
	{
		this->operator()(attrname, value);
	}

	configr_of(const config& cfg) : subtags_(), data_()
	{
		this->operator()(cfg);
	}

	configr_of& operator()(const config& cfg)
	{
		data_ = &cfg;
		return *this;
	}

	configr_of& operator()(const std::string& tagname, const configr_of& child)
	{
		subtags_.emplace_back(&tagname, &child);
		return *this;
	}
	std::vector<std::pair<const std::string*, const configr_of*>> subtags_;
	const config* data_;
};
