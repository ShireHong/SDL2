#pragma once

#include <SDL2/SDL_events.h>

#include <cstring>

namespace sdl
{

class UserEvent
{
public:
	UserEvent()
	{
		std::memset(&event_, 0, sizeof(event_));
	}

	UserEvent(int type) : UserEvent()
	{
		event_.type = type;
	}

	UserEvent(int type, int code) : UserEvent(type)
	{
		event_.code = code;
	}

	UserEvent(int type, int code, std::size_t data1, std::size_t data2) : UserEvent(type)
	{
		event_.code = code;
		event_.data1 = reinterpret_cast<void*>(data1);
		event_.data2 = reinterpret_cast<void*>(data2);
	}

	UserEvent(int type, void* data1) : UserEvent(type)
	{
		event_.data1 = data1;
	}

	operator SDL_UserEvent()
	{
		return event_;
	}

private:
	SDL_UserEvent event_;
};

}
