#pragma once

#include <SDL2/SDL_events.h>
#include <vector>
#include <list>
#include <functional>

//our user-defined double-click event type
#define DOUBLE_CLICK_EVENT SDL_USEREVENT
#define TIMER_EVENT (SDL_USEREVENT + 1)
#define HOVER_REMOVE_POPUP_EVENT (SDL_USEREVENT + 2)
#define DRAW_EVENT (SDL_USEREVENT + 3)
#define CLOSE_WINDOW_EVENT (SDL_USEREVENT + 4)
#define SHOW_HELPTIP_EVENT (SDL_USEREVENT + 5)
#define INVOKE_FUNCTION_EVENT (SDL_USEREVENT + 6)


namespace events
{

class sdl_handler;

typedef std::list<sdl_handler*> handler_list;
class context
{
public:
	context() :
		handlers(),
		staging_handlers()
	{
	}

	~context();

	context(const context&) = delete;

	void add_handler(sdl_handler* ptr);
	void add_staging_handlers();


	handler_list handlers;
	std::vector<sdl_handler*> staging_handlers;

};

class sdl_handler
{
friend class context;
public:
	virtual void handle_event(const SDL_Event& event) = 0;
    virtual void process_tooltip_string(int /*mousex*/, int /*mousey*/) {}
    virtual void process_event() {}
    virtual bool has_joined() { return has_joined_;}
	virtual bool has_joined_global() { return has_joined_global_;}

protected:
	sdl_handler(const bool auto_join=true);
	virtual ~sdl_handler();

private:
	bool has_joined_;
	bool has_joined_global_;
};

struct event_context
{
	event_context();
	~event_context();
};

/** Process all events currently in the queue. */
void pump();

/** Trigger a draw cycle. */
void draw();

/**
 * Triggered by mouse-motion, sends the cursor position to all handlers to
 * check whether a tooltip should be shown.
 */
void process_tooltip_strings(int mousex, int mousey);


/**
 * Is the event an input event?
 *
 * @returns                       Whether or not the event is an input event.
 */
bool is_input(const SDL_Event& event);

/** Discards all input events. */
void discard_input();

}

typedef std::vector<events::sdl_handler*> sdl_handler_vector;