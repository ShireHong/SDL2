

#include "sdl/point.h"

#include <ostream>

std::ostream& operator<<(std::ostream& stream, const point& point)
{
	stream << '(' << point.x << ',' << point.y << ')';
	return stream;
}
