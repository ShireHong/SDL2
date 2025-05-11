
// #include "serialization/string_utils.hpp"
#include "color.h"

#include <iomanip>
#include <sstream>

// color_t color_t::from_rgba_string(const std::string& c)
// {
// 	if(c.empty()) {
// 		return null_color();
// 	}

// 	std::vector<std::string> fields = utils::split(c);

// 	// Allow either 3 (automatic opaque alpha) or 4 (explicit alpha) fields
// 	if(fields.size() != 3 && fields.size() != 4) {
// 		throw std::invalid_argument("Wrong number of components for RGBA color");
// 	}

// 	return {
// 		static_cast<uint8_t>(std::stoul(fields[0])),
// 		static_cast<uint8_t>(std::stoul(fields[1])),
// 		static_cast<uint8_t>(std::stoul(fields[2])),
// 		static_cast<uint8_t>(fields.size() == 4 ? std::stoul(fields[3]) : ALPHA_OPAQUE)
// 	};
// }

// color_t color_t::from_rgb_string(const std::string& c)
// {
// 	if(c.empty()) {
// 		return null_color();
// 	}

// 	std::vector<std::string> fields = utils::split(c);

// 	if(fields.size() != 3) {
// 		throw std::invalid_argument("Wrong number of components for RGB color");
// 	}

// 	return {
// 		static_cast<uint8_t>(std::stoul(fields[0])),
// 		static_cast<uint8_t>(std::stoul(fields[1])),
// 		static_cast<uint8_t>(std::stoul(fields[2])),
// 		static_cast<uint8_t>(ALPHA_OPAQUE)
// 	};
// }

color_t color_t::from_hex_string(const std::string& c)
{
	if(c.length() != 6) {
		throw std::invalid_argument("Color hex string should be exactly 6 digits");
	}

	unsigned long temp_c = std::strtol(c.c_str(), nullptr, 16);

	return {
		static_cast<uint8_t>((0x00FFFFFF & temp_c) >> 16),
		static_cast<uint8_t>((0x00FFFFFF & temp_c) >> 8),
		static_cast<uint8_t>((0x00FFFFFF & temp_c)),
		ALPHA_OPAQUE
	};
}

std::string color_t::to_hex_string() const
{
	std::ostringstream h;

	h << "#"
	  << std::hex << std::setfill('0')
	  << std::setw(2) << static_cast<int>(r)
	  << std::setw(2) << static_cast<int>(g)
	  << std::setw(2) << static_cast<int>(b);

	if(a != ALPHA_OPAQUE) {
		h << std::setw(2) << static_cast<int>(a);
	}

	return h.str();
}

std::string color_t::to_rgba_string() const
{
	std::ostringstream color;

	color << static_cast<int>(r) << ','
	      << static_cast<int>(g) << ','
	      << static_cast<int>(b) << ','
	      << static_cast<int>(a);

	return color.str();
}

std::string color_t::to_rgb_string() const
{
	std::ostringstream color;

	color << static_cast<int>(r) << ','
	      << static_cast<int>(g) << ','
	      << static_cast<int>(b);

	return color.str();
}
