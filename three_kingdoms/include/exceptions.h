
#pragma once

#include <exception>
#include <string>

namespace game {

/**
 * Base class for all the errors encountered by the engine.
 * It provides a field for storing custom messages related to the actual
 * error.
 */
struct error : std::exception
{
	std::string message;

	error() : message() {}
	error(const std::string &msg) : message(msg) {}
	~error() noexcept {}

	const char *what() const noexcept
	{
		return message.c_str();
	}
};

}
