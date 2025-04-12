/*
	Copyright (C) 2003 - 2023
	by David White <dave@whitevine.net>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "events.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <deque>
#include <future>
#include <iterator>
#include <thread>
#include <utility>
#include <vector>
#include "draw_manager.h"
#include <SDL2/SDL.h>


namespace events
{
void context::add_handler(sdl_handler* ptr)
{
	/* Add new handlers to the staging list initially.
	 * This ensures that if an event handler adds more handlers, the new handlers
	 * won't be called for the event that caused them to be added.
	 */
	staging_handlers.push_back(ptr);
}

void context::add_staging_handlers()
{
	std::copy(staging_handlers.begin(), staging_handlers.end(), std::back_inserter(handlers));
	staging_handlers.clear();
}

context::~context()
{
	for(sdl_handler* h : handlers) {
		if(h->has_joined()) {
			h->has_joined_ = false;
		}

		if(h->has_joined_global()) {
			h->has_joined_global_ = false;
		}
	}
}

// This object stores all the event handlers. It is a stack of event 'contexts'.
// a new event context is created when e.g. a modal dialog is opened, and then
// closed when that dialog is closed. Each context contains a list of the handlers
// in that context. The current context is the one on the top of the stack.
// The global context must always be in the first position.
std::deque<context> event_contexts;

event_context::event_context()
{
	event_contexts.emplace_back();
}

event_context::~event_context()
{
	assert(event_contexts.empty() == false);
	event_contexts.pop_back();
}

sdl_handler::sdl_handler(const bool auto_join)
	: has_joined_(false)
	, has_joined_global_(false)
{
	if(auto_join) {
		assert(!event_contexts.empty());
		event_contexts.back().add_handler(this);
		has_joined_ = true;
	}
}

sdl_handler::~sdl_handler()
{
	if(has_joined_) {
		//leave();
	}

	if(has_joined_global_) {
		//leave_global();
	}
}

void pump()
{
	// Used to keep track of double click events
	static int last_mouse_down = -1;
	static int last_click_x = -1, last_click_y = -1;

	SDL_Event temp_event;
	int poll_count = 0;
	int begin_ignoring = 0;

	std::vector<SDL_Event> events;
	while(SDL_PollEvent(&temp_event)) 
    {
		if(temp_event.type == INVOKE_FUNCTION_EVENT) {
			//static_cast<invoked_function_data*>(temp_event.user.data1)->call();
			continue;
		}

		++poll_count;

		if(!begin_ignoring && temp_event.type == SDL_WINDOWEVENT && (
			temp_event.window.event == SDL_WINDOWEVENT_ENTER ||
			temp_event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
		) {
			begin_ignoring = poll_count;
		} else if(begin_ignoring > 0 && is_input(temp_event)) {
			// ignore user input events that occurred after the window was activated
			continue;
		}

		events.push_back(temp_event);
	}

	auto ev_it = events.begin();
	for(int i = 1; i < begin_ignoring; ++i) {
		if(is_input(*ev_it)) {
			// ignore user input events that occurred before the window was activated
			ev_it = events.erase(ev_it);
		} else {
			++ev_it;
		}
	}

	for(SDL_Event& event : events) {
		for(context& c : event_contexts) {
			c.add_staging_handlers();
		}

		switch(event.type) {
		case SDL_MOUSEMOTION: {
			// Always make sure a cursor is displayed if the mouse moves or if the user clicks
			//cursor::set_focus(true);
			process_tooltip_strings(event.motion.x, event.motion.y);

			break;
		}

		case SDL_MOUSEBUTTONDOWN: {
			// Always make sure a cursor is displayed if the mouse moves or if the user clicks
			//cursor::set_focus(true);
			if(event.button.button == SDL_BUTTON_LEFT || event.button.which == SDL_TOUCH_MOUSEID) {
				static const int DoubleClickTime = 500;

				// if(last_mouse_down >= 0 && info.ticks() - last_mouse_down < DoubleClickTime
				// 		&& std::abs(event.button.x - last_click_x) < DoubleClickMaxMove
				// 		&& std::abs(event.button.y - last_click_y) < DoubleClickMaxMove
				// ) {
				// 	sdl::UserEvent user_event(DOUBLE_CLICK_EVENT, event.button.which, event.button.x, event.button.y);
				// 	::SDL_PushEvent(reinterpret_cast<SDL_Event*>(&user_event));
				// }

				// last_mouse_down = info.ticks();
				// last_click_x = event.button.x;
				// last_click_y = event.button.y;
			}
			break;
		}


		case SDL_SYSWMEVENT: {
			//windows_tray_notification::handle_system_event(event);
			break;
		}


		case SDL_QUIT: {
			//quit_confirmation::quit_to_desktop();
			continue; // this event is already handled.
		}
		}

		for(auto global_handler : event_contexts.front().handlers) {
			global_handler->handle_event(event);
		}

		if(event_contexts.empty() == false) {
			// As pump() can recurse, pretty much anything can happen here
			// including destroying handlers or the event context.
			size_t ec_index = event_contexts.size();
			context& c = event_contexts.back();
			handler_list& h = c.handlers;
			size_t h_size = h.size();
			for(auto it = h.begin(); it != h.end(); ++it) {
				// Pass the event on to the handler.
				(*it)->handle_event(event);
				// Escape if anything has changed.
				if(event_contexts.size() != ec_index) {
					//LOG_EV << "ec size changed! bugging out";
					break;
				}
				if(h_size != h.size()) {
					//LOG_EV << "h size changed! bugging out";
					break;
				}
			}
		}
	}

	// Inform the pump monitors that an events::pump() has occurred

}

void draw()
{
    //std::cout<<"draw"<<std::endl;
	draw_manager::sparkle();
}

void raise_process_event()
{
	if(event_contexts.empty() == false) {
		event_contexts.back().add_staging_handlers();

		for(auto handler : event_contexts.back().handlers) {
			handler->process_event();
		}
	}
}

void process_tooltip_strings(int mousex, int mousey)
{
	if(event_contexts.empty() == false) {
		for(auto handler : event_contexts.back().handlers) {
			handler->process_tooltip_string(mousex, mousey);
		}
	}
}

/* The constants for the minimum and maximum are picked from the headers. */
#define INPUT_MIN 0x300
#define INPUT_MAX 0x8FF

bool is_input(const SDL_Event& event)
{
	return event.type >= INPUT_MIN && event.type <= INPUT_MAX;
}

void discard_input()
{
	SDL_FlushEvents(INPUT_MIN, INPUT_MAX);
}

} // end events namespace
