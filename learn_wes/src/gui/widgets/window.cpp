
#include "gui/widgets/window_private.h"

#include "config.h"
#include "draw.h"
#include "events.h"
// #include "formula/callable.hpp"
// #include "formula/string_utils.hpp"
// #include "gettext.hpp"
// #include "log.hpp"
// #include "gui/auxiliary/typed_formula.hpp"
#include "gui/auxiliary/find_widget.h"
#include "gui/core/event/distributor.h"
#include "gui/core/event/handler.h"
#include "gui/core/event/message.h"
// #include "gui/core/log.hpp"
#include "gui/core/layout_exception.h"
#include "sdl/point.h"
#include "gui/core/window_builder.h"
// #include "gui/dialogs/tooltip.hpp"
#include "gui/widgets/button.h"
#include "gui/widgets/container_base.h"
// #include "gui/widgets/text_box_base.hpp"
#include "gui/core/register_widget.h"
#include "gui/widgets/grid.h"
#include "gui/widgets/helper.h"
#include "gui/widgets/panel.h"
#include "gui/widgets/settings.h"
#include "gui/widgets/widget.h"
#include "gui/widgets/window.h"
// #ifdef DEBUG_WINDOW_LAYOUT_GRAPHS
// #include "gui/widgets/debug.hpp"
// #endif
#include "sdl/rect.h"
#include "sdl/texture.h"
// #include "formula/variant.hpp"
#include "video.h" // only for toggle_fullscreen
// #include "wml_exception.hpp"
#include "sdl/userevent.h"
#include "sdl/input.h" // get_mouse_button_mask

#include <functional>
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <cassert>
#include <sstream>
// static lg::log_domain log_gui("gui/layout");
// #define ERR_GUI  LOG_STREAM(err, log_gui)

// #define LOG_SCOPE_HEADER get_control_type() + " [" + id() + "] " + __func__
#define LOG_HEADER "windows" + ':'

// #define LOG_IMPL_SCOPE_HEADER                                                  \
// 	window.get_control_type() + " [" + window.id() + "] " + __func__
// #define LOG_IMPL_HEADER LOG_IMPL_SCOPE_HEADER + ':'

// static lg::log_domain log_display("display");
// #define DBG_DP LOG_STREAM(debug, log_display)
// #define LOG_DP LOG_STREAM(info, log_display)
// #define WRN_DP LOG_STREAM(warn, log_display)

namespace gui2
{

// ------------ WIDGET -----------{

namespace implementation
{
/** @todo See whether this hack can be removed. */
// Needed to fix a compiler error in REGISTER_WIDGET.
class builder_window : public builder_styled_widget
{
public:
	builder_window() : builder_styled_widget()
	{
	}

	using builder_styled_widget::build;

	virtual std::unique_ptr<widget> build() const override
	{
		return nullptr;
	}
};

} // namespace implementation
REGISTER_WIDGET(window)

namespace
{
#ifdef DEBUG_WINDOW_LAYOUT_GRAPHS
const unsigned SHOW = debug_layout_graph::SHOW;
const unsigned LAYOUT = debug_layout_graph::LAYOUT;
#else
// values are irrelavant when DEBUG_WINDOW_LAYOUT_GRAPHS is not defined.
const unsigned SHOW = 0;
const unsigned LAYOUT = 0;
#endif

/**
 * SDL_AddTimer() callback for delay_event.
 *
 * @param event                   The event to push in the event queue.
 *
 * @return                        The new timer interval (always 0).
 */
static uint32_t delay_event_callback(const uint32_t, void* event)
{
	SDL_PushEvent(static_cast<SDL_Event*>(event));
	delete static_cast<SDL_Event*>(event);
	return 0;
}

/**
 * Allows an event to be delayed a certain amount of time.
 *
 * @note the delay is the minimum time, after the time has passed the event
 * will be pushed in the SDL event queue, so it might delay more.
 *
 * @param event                   The event to delay.
 * @param delay                   The number of ms to delay the event.
 */
static void delay_event(const SDL_Event& event, const uint32_t delay)
{
	SDL_AddTimer(delay, delay_event_callback, new SDL_Event(event));
}

/**
 * Adds a SHOW_HELPTIP event to the SDL event queue.
 *
 * The event is used to show the helptip for the currently focused widget.
 */
static void helptip()
{
	// DBG_GUI_E << "Pushing SHOW_HELPTIP_EVENT event in queue.";
    std::cout << "Pushing SHOW_HELPTIP_EVENT event in queue." << std::endl;
	SDL_Event event;
	sdl::UserEvent data(SHOW_HELPTIP_EVENT);

	event.type = SHOW_HELPTIP_EVENT;
	event.user = data;

	SDL_PushEvent(&event);
}

/**
 * Small helper class to get an unique id for every window instance.
 *
 * This is used to send event to the proper window, this allows windows to post
 * messages to themselves and let them delay for a certain amount of time.
 */
class manager
{
	manager();

public:
	static manager& instance();

	void add(window& window);

	void remove(window& window);

	unsigned get_id(window& window);

	window* get_window(const unsigned id);

private:
	// The number of active window should be rather small
	// so keep it simple and don't add a reverse lookup map.
	std::map<unsigned, window*> windows_;
};

manager::manager() : windows_()
{
}

manager& manager::instance()
{
	static manager window_manager;
	return window_manager;
}

void manager::add(window& win)
{
	static unsigned id;
	++id;
	windows_[id] = &win;
}

void manager::remove(window& win)
{
	for(std::map<unsigned, window*>::iterator itor = windows_.begin();
		itor != windows_.end();
		++itor) {

		if(itor->second == &win) {
			windows_.erase(itor);
			return;
		}
	}
	assert(false);
}

unsigned manager::get_id(window& win)
{
	for(std::map<unsigned, window*>::iterator itor = windows_.begin();
		itor != windows_.end();
		++itor) {

		if(itor->second == &win) {
			return itor->first;
		}
	}
	assert(false);

	return 0;
}

window* manager::get_window(const unsigned id)
{
	std::map<unsigned, window*>::iterator itor = windows_.find(id);

	if(itor == windows_.end()) {
		return nullptr;
	} else {
		return itor->second;
	}
}

} // namespace

window::window(const builder_window::window_resolution& definition)
	: panel(implementation::builder_window(), type())
	, status_(status::NEW)
	, show_mode_(show_mode::none)
	, retval_(retval::NONE)
	, owner_(nullptr)
	, need_layout_(true)
	// , variables_()
	, invalidate_layout_blocked_(false)
	, hidden_(true)
	, automatic_placement_(definition.automatic_placement)
	, horizontal_placement_(definition.horizontal_placement)
	, vertical_placement_(definition.vertical_placement)
	// , maximum_width_(definition.maximum_width)
	// , maximum_height_(definition.maximum_height)
	// , x_(definition.x)
	// , y_(definition.y)
	// , w_(definition.width)
	// , h_(definition.height)
	// , reevaluate_best_size_(definition.reevaluate_best_size)
	// , functions_(definition.functions)
	// , tooltip_(definition.tooltip)
	// , helptip_(definition.helptip)
	, click_dismiss_(false)
	, enter_disabled_(false)
	, escape_disabled_(false)
	, linked_size_()
	, mouse_button_state_(0) /**< Needs to be initialized in @ref show. */
#ifdef DEBUG_WINDOW_LAYOUT_GRAPHS
	, debug_layout_(new debug_layout_graph(this))
#endif
	// , event_distributor_(new event::distributor(*this, event::dispatcher::front_child))
	, exit_hook_([](window&)->bool { return true; })
{
	std::cout<<"window create!"<<std::endl;
	manager::instance().add(*this);

	connect();

	// connect_signal<event::SDL_VIDEO_RESIZE>(std::bind(
	// 		&window::signal_handler_sdl_video_resize, this, std::placeholders::_2, std::placeholders::_3, std::placeholders::_5));

	// connect_signal<event::SDL_ACTIVATE>(std::bind(
	// 		&event::distributor::initialize_state, event_distributor_.get()));

	connect_signal<event::SDL_LEFT_BUTTON_UP>(
			std::bind(&window::signal_handler_click_dismiss,
						this,
						std::placeholders::_2,
						std::placeholders::_3,
						std::placeholders::_4,
						SDL_BUTTON_LMASK),
			event::dispatcher::front_child);
	connect_signal<event::SDL_MIDDLE_BUTTON_UP>(
			std::bind(&window::signal_handler_click_dismiss,
						this,
						std::placeholders::_2,
						std::placeholders::_3,
						std::placeholders::_4,
						SDL_BUTTON_MMASK),
			event::dispatcher::front_child);
	connect_signal<event::SDL_RIGHT_BUTTON_UP>(
			std::bind(&window::signal_handler_click_dismiss,
						this,
						std::placeholders::_2,
						std::placeholders::_3,
						std::placeholders::_4,
						SDL_BUTTON_RMASK),
			event::dispatcher::front_child);

	// connect_signal<event::SDL_KEY_DOWN>(
	// 		std::bind(
	// 				&window::signal_handler_sdl_key_down, this, std::placeholders::_2, std::placeholders::_3, std::placeholders::_5, std::placeholders::_6, true),
	// 		event::dispatcher::back_post_child);
	// connect_signal<event::SDL_KEY_DOWN>(std::bind(
	// 		&window::signal_handler_sdl_key_down, this, std::placeholders::_2, std::placeholders::_3, std::placeholders::_5, std::placeholders::_6, false));

	// connect_signal<event::MESSAGE_SHOW_TOOLTIP>(
	// 		std::bind(&window::signal_handler_message_show_tooltip,
	// 					this,
	// 					std::placeholders::_2,
	// 					std::placeholders::_3,
	// 					std::placeholders::_5),
	// 		event::dispatcher::back_pre_child);

	// connect_signal<event::MESSAGE_SHOW_HELPTIP>(
	// 		std::bind(&window::signal_handler_message_show_helptip,
	// 					this,
	// 					std::placeholders::_2,
	// 					std::placeholders::_3,
	// 					std::placeholders::_5),
	// 		event::dispatcher::back_pre_child);

	// connect_signal<event::REQUEST_PLACEMENT>(
	// 		std::bind(
	// 				&window::signal_handler_request_placement, this, std::placeholders::_2, std::placeholders::_3),
	// 		event::dispatcher::back_pre_child);

	// connect_signal<event::CLOSE_WINDOW>(std::bind(&window::signal_handler_close_window, this));

	// register_hotkey(hotkey::GLOBAL__HELPTIP, std::bind(gui2::helptip));

	// /** @todo: should eventally become part of global hotkey handling. */
	// register_hotkey(hotkey::HOTKEY_FULLSCREEN,
	// 	std::bind(&video::toggle_fullscreen));
}

window::~window()
{
	/*
	 * We need to delete our children here instead of waiting for the grid to
	 * automatically do it. The reason is when the grid deletes its children
	 * they will try to unregister them self from the linked widget list. At
	 * this point the member of window are destroyed and we enter UB. (For
	 * some reason the bug didn't trigger on g++ but it does on MSVC.
	 */
	// for(unsigned row = 0; row < get_grid().get_rows(); ++row) {
	// 	for(unsigned col = 0; col < get_grid().get_cols(); ++col) {
	// 		get_grid().remove_child(row, col);
	// 	}
	// }

	/*
	 * The tip needs to be closed if the window closes and the window is
	 * not a tip. If we don't do that the tip will unrender in the next
	 * window and cause drawing glitches.
	 * Another issue is that on smallgui and an MP game the tooltip not
	 * unrendered properly can capture the mouse and make playing impossible.
	 */
	// if(show_mode_ == show_mode::modal) {
	// 	dialogs::tip::remove();
	// }

	manager::instance().remove(*this);

	// If we are currently shown, then queue an undraw.
	if(!hidden_) {
		queue_redraw();
	}

#ifdef DEBUG_WINDOW_LAYOUT_GRAPHS

	delete debug_layout_;

#endif
}

window* window::window_instance(const unsigned handle)
{
	return manager::instance().get_window(handle);
}

retval window::get_retval_by_id(const std::string& id)
{
	// Note it might change to a map later depending on the number
	// of items.
	if(id == "ok") {
		return retval::OK;
	} else if(id == "cancel" || id == "quit") {
		return retval::CANCEL;
	} else {
		return retval::NONE;
	}
}

void window::finish_build(const builder_window::window_resolution& definition)
{
	// for(const auto& lg : definition.linked_groups) {
	// 	if(has_linked_size_group(lg.id)) {
	// 		t_string msg = VGETTEXT("Linked '$id' group has multiple definitions.", {{"id", lg.id}});

	// 		FAIL(msg);
	// 	}

	// 	init_linked_size_group(lg.id, lg.fixed_width, lg.fixed_height);
	// }

	set_click_dismiss(definition.click_dismiss);

	//window_definition win_def(win_resol);
    const auto conf = cast_config_to<window_definition>();
	// assert(conf);

	// if(conf->grid) {
	// 	init_grid(*conf->grid);
	// 	finalize(*definition.grid);
	// } else {
	// 	init_grid(*definition.grid);
	// }

	add_to_keyboard_chain(this);
}

void window::show_tooltip(/*const unsigned auto_close_timeout*/)
{
	// Unhide in any case.
	hidden_ = false;

	// Connect to the event handler, if not yet connected.
	// if(!is_connected()) {
	// 	LOG_DP << "connecting " << id() << " on show_tooltip";
	// 	connect();
	// }

	// log_scope2(log_gui_draw, "Window: show as tooltip.");

	generate_dot_file("show", SHOW);

	assert(status_ == status::NEW);

	// set_mouse_behavior(event::dispatcher::mouse_behavior::none);
	// set_want_keyboard_input(false);

	show_mode_ = show_mode::tooltip;

	/*
	 * Before show has been called, some functions might have done some testing
	 * on the window and called layout, which can give glitches. So
	 * reinvalidate the window to avoid those glitches.
	 */
	invalidate_layout();
	queue_redraw();
	// DBG_DP << "show tooltip queued to " << get_rectangle();
    std::cout << "show tooltip queued to " << get_rectangle() << std::endl;
}

void window::show_non_modal(/*const unsigned auto_close_timeout*/)
{
	// Unhide in any case.
	hidden_ = false;

	// Connect to the event handler, if not yet connected.
	// if(!is_connected()) {
	// 	LOG_DP << "connecting " << id() << " on show_non_modal";
	// 	connect();
	// }

	// log_scope2(log_gui_draw, "Window: show non modal.");

	generate_dot_file("show", SHOW);

	assert(status_ == status::NEW);

	// set_mouse_behavior(event::dispatcher::mouse_behavior::hit);

	show_mode_ = show_mode::modeless;

	/*
	 * Before show has been called, some functions might have done some testing
	 * on the window and called layout, which can give glitches. So
	 * reinvalidate the window to avoid those glitches.
	 */
	invalidate_layout();
	queue_redraw();

	// DBG_DP << "show non-modal queued to " << get_rectangle();

	// events::pump_and_draw();
}

int window::show(const unsigned auto_close_timeout)
{
	// Unhide in any case.
	hidden_ = false;

	// Connect to the event handler, if not yet connected.
	// if(!is_connected()) {
	// 	LOG_DP << "connecting " << id() << " on show";
	// 	connect();
	// }

	/*
	 * Removes the old tip if one shown. The show_tip doesn't remove
	 * the tip, since it's the tip.
	 */
	// dialogs::tip::remove();

	show_mode_ = show_mode::modal;

	// log_scope2(log_gui_draw, LOG_SCOPE_HEADER);

	generate_dot_file("show", SHOW);

	//assert(status_ == status::NEW);

	/*
	 * Before show has been called, some functions might have done some testing
	 * on the window and called layout, which can give glitches. So
	 * reinvalidate the window to avoid those glitches.
	 */
	invalidate_layout();
	queue_redraw();

	// Make sure we display at least once in all cases.
	events::draw();

	if(auto_close_timeout) {
		SDL_Event event;
		sdl::UserEvent data(CLOSE_WINDOW_EVENT, manager::instance().get_id(*this));

		event.type = CLOSE_WINDOW_EVENT;
		event.user = data;

		delay_event(event, auto_close_timeout);
	}

	try
	{
		// According to the comment in the next loop, we need to pump() once
		// before we know which mouse buttons are down. Assume they're all
		// down, otherwise there's a race condition when the MOUSE_UP gets
		// processed in the first pump(), which immediately closes the window.
		bool mouse_button_state_initialized = false;
		mouse_button_state_ = std::numeric_limits<uint32_t>::max();
		
		// Start our loop, drawing will happen here as well.
		for(status_ = status::SHOWING; status_ != status::CLOSED;) {
			// Process and handle all pending events.
			events::pump();

			if(!mouse_button_state_initialized) {
				/*
				 * The state must be initialize when showing the dialog.
				 * However when initialized before this point there were random
				 * errors. This only happened when the 'click' was done fast; a
				 * slower click worked properly.
				 *
				 * So it seems the events need to be processed before SDL can
				 * return the proper button state. When initializing here all
				 * works fine.
				 */
				mouse_button_state_ = sdl::get_mouse_button_mask();
				mouse_button_state_initialized = true;
			}

			// See if we should close.
			if(status_ == status::REQUEST_CLOSE) {
				status_ = exit_hook_(*this) ? status::CLOSED : status::SHOWING;
			}

			// Update the display. This will rate limit to vsync.
			events::draw();
		}
	}
	catch(...)
	{
		// TODO: is this even necessary? What are we catching?
		// DBG_DP << "Caught general exception in show(): " << utils::get_unknown_exception_type();
		hide();
		throw;
	}

	// if(text_box_base* tb = dynamic_cast<text_box_base*>(event_distributor_->keyboard_focus())) {
	// 	tb->interrupt_composition();
	// }

	// The window may be kept around to be re-shown later. Hide it for now.
	hide();

	return retval_;
}

void window::draw()
{
	if(hidden_) {
		return;
	}

	// Draw background.
	if(!this->draw_background()) {
		// We may need to blur the background behind the window,
		// but at this point it hasn't been rendered yet.
		// We thus defer rendering to next frame so we can snapshot what
		// is underneath the window without drawing over it.
		defer_region(get_rectangle());
		return;
	}

	// Draw children.
	this->draw_children();

	// Draw foreground.
	if(!this->draw_foreground()) {
		defer_region(get_rectangle());
	}

	return;
}

void window::hide()
{
	// Queue a redraw of the region if we were shown.
	if(!hidden_) {
		queue_redraw();
	}

	// Disconnect from the event handler so we stop receiving events.
	// if(is_connected()) {
	// 	LOG_DP << "disconnecting " << id() << " on hide";
	// 	disconnect();
	// }

	hidden_ = true;
}

void window::update_render_textures()
{
	point draw = get_size();
	point render = draw * video::get_pixel_scale();

	// Check that the render buffer size is correct.
	point buf_raw = render_buffer_.get_raw_size();
	point buf_draw = render_buffer_.draw_size();
	bool raw_size_changed = buf_raw.x != render.x || buf_raw.y != render.y;
	bool draw_size_changed = buf_draw.x != draw.x || buf_draw.y != draw.y;
	if (!raw_size_changed && !draw_size_changed) {
		// buffers are fine
		return;
	}

	if(raw_size_changed) {
		// LOG_DP << "regenerating window render buffer as " << render;
        std::cout << "regenerating window render buffer as " << render << std::endl;
		render_buffer_ = texture(render.x, render.y, SDL_TEXTUREACCESS_TARGET);
	}
	if(raw_size_changed || draw_size_changed) {
		// LOG_DP << "updating window render buffer draw size to " << draw;
        std::cout << "updating window render buffer draw size to " << draw << std::endl;
		render_buffer_.set_draw_size(draw); 
	}

	// Clear the entire texture.
	// {
	// 	auto setter = draw::set_render_target(render_buffer_);
	// 	draw::clear();
	// }

	// Rerender everything.
	queue_rerender();
}

void window::queue_rerender()
{
	queue_rerender(get_rectangle());
}

void window::queue_rerender(const rect& screen_region)
{
	// More than one region updating per-frame should be rare.
	// Just rerender the minimal area that covers everything.
	rect local_region = screen_region.intersect(get_rectangle());
	local_region.shift(-get_origin());
	awaiting_rerender_.expand_to_cover(local_region);
}

void window::defer_region(const rect& screen_region)
{
	// LOG_DP << "deferring region " << screen_region;
    std::cout << "deferring region " << screen_region << std::endl;
	deferred_regions_.push_back(screen_region);
}

void window::render()
{
	// Ensure our render texture is correctly sized.
	update_render_textures();

	// Mark regions that were previously deferred for rerender and repaint.
	for(auto& region : deferred_regions_) {
		queue_redraw(region);
	}
	deferred_regions_.clear();

	// Render the portion of the window awaiting rerender (if any).
	if (awaiting_rerender_.empty()) {
		return;
	}
	// DBG_DP << "window::render() local " << awaiting_rerender_;
    std::cout << "window::render() local " << awaiting_rerender_ << std::endl;
	// auto target_setter = draw::set_render_target(render_buffer_);
	// auto clip_setter = draw::override_clip(awaiting_rerender_);
	draw();
	awaiting_rerender_ = sdl::empty_rect;
}

bool window::expose(const rect& region)
{
	// DBG_DP << "window::expose " << region;
    std::cout << "window::expose " << region << std::endl;

	// Calculate the destination region we need to draw.
	rect dst = get_rectangle().intersect(region);
	// dst.clip(draw::get_clip());
	if (dst.empty()) {
		return false;
	}

	// Blit from the pre-rendered buffer.
	rect src = dst;
	src.shift(-get_origin());
	render_buffer_.set_src(src);
	// draw::blit(render_buffer_, dst);
	render_buffer_.clear_src();

	return true;
}

rect window::screen_location()
{
	if(hidden_) {
		return {0,0,0,0};
	}
	return get_rectangle();
}

window::invalidate_layout_blocker::invalidate_layout_blocker(window& window)
	: window_(window)
{
	assert(!window_.invalidate_layout_blocked_);
	window_.invalidate_layout_blocked_ = true;
}

window::invalidate_layout_blocker::~invalidate_layout_blocker()
{
	assert(window_.invalidate_layout_blocked_);
	window_.invalidate_layout_blocked_ = false;
}

void window::invalidate_layout()
{
	if(!invalidate_layout_blocked_) {
		need_layout_ = true;
	}
}
widget* window::find_at(const point& coordinate, const bool must_be_active)
{
	return panel::find_at(coordinate, must_be_active);
}

const widget* window::find_at(const point& coordinate,
								const bool must_be_active) const
{
	return panel::find_at(coordinate, must_be_active);
}

widget* window::find(const std::string& id, const bool must_be_active)
{
	return container_base::find(id, must_be_active);
}

const widget* window::find(const std::string& id, const bool must_be_active)
		const
{
	return container_base::find(id, must_be_active);
}

void window::init_linked_size_group(const std::string& id,
									 const bool fixed_width,
									 const bool fixed_height)
{
	assert(fixed_width || fixed_height);
	assert(!has_linked_size_group(id));

	linked_size_[id] = linked_size(fixed_width, fixed_height);
}

bool window::has_linked_size_group(const std::string& id)
{
	return linked_size_.find(id) != linked_size_.end();
}

void window::add_linked_widget(const std::string& id, widget* wgt)
{
	assert(wgt);
	if(!has_linked_size_group(id)) {
		// ERR_GUI << "Unknown linked group '" << id << "'; skipping";
        std::cout << "Unknown linked group '" << id << "'; skipping" << std::endl;
		return;
	}

	std::vector<widget*>& widgets = linked_size_[id].widgets;
	if(std::find(widgets.begin(), widgets.end(), wgt) == widgets.end()) {
		widgets.push_back(wgt);
	}
}

void window::remove_linked_widget(const std::string& id, const widget* wgt)
{
	assert(wgt);
	if(!has_linked_size_group(id)) {
		return;
	}

	std::vector<widget*>& widgets = linked_size_[id].widgets;

	std::vector<widget*>::iterator itor
			= std::find(widgets.begin(), widgets.end(), wgt);

	if(itor != widgets.end()) {
		widgets.erase(itor);

		assert(std::find(widgets.begin(), widgets.end(), wgt)
			   == widgets.end());
	}
}

void window::layout()
{
	if(!need_layout_) {
		return;
	}
	// DBG_DP << "window::layout";
    std::cout << "window::layout" << std::endl;

	/***** Initialize. *****/

	const auto conf = cast_config_to<window_definition>();
	assert(conf);

	// log_scope2(log_gui_layout, LOG_SCOPE_HEADER);

	point size = get_best_size();
	std::cout << "get_best_size" << size << std::endl;
	const point mouse = get_mouse_position();

	// variables_.add("mouse_x", wfl::variant(mouse.x));
	// variables_.add("mouse_y", wfl::variant(mouse.y));
	// variables_.add("window_width", wfl::variant(0));
	// variables_.add("window_height", wfl::variant(0));
	// variables_.add("best_window_width", wfl::variant(size.x));
	// variables_.add("best_window_height", wfl::variant(size.y));
	// variables_.add("size_request_mode", wfl::variant("maximum"));

	// get_screen_size_variables(variables_);

	// unsigned int maximum_width = maximum_width_(variables_, &functions_);
	// unsigned int maximum_height = maximum_height_(variables_, &functions_);
	unsigned int maximum_width = 900;
	unsigned int maximum_height = 800;
	if(automatic_placement_) {
		if(maximum_width == 0 || maximum_width > settings::screen_width) {
			maximum_width = settings::screen_width;
		}

	// 	if(maximum_height == 0 || maximum_height > settings::screen_height) {
	// 		maximum_height = settings::screen_height;
	// 	}
	}
	// } else {
	// 	maximum_width  = w_(variables_, &functions_);
	// 	maximum_height = h_(variables_, &functions_);
	// }

	// /***** Handle click dismiss status. *****/
	button* click_dismiss_button = nullptr;
	if((click_dismiss_button
		= find_widget<button>(this, "click_dismiss", false, false))) {

		click_dismiss_button->set_visible(widget::visibility::invisible);
	}
	if(click_dismiss_) {
		button* btn = find_widget<button>(this, "ok", false, false);
		if(btn) {
			btn->set_visible(widget::visibility::invisible);
			click_dismiss_button = btn;
		}
		// VALIDATE(click_dismiss_button,
		// 		 _("Click dismiss needs a 'click_dismiss' or 'ok' button."));
	}

	/***** Layout. *****/
	layout_initialize(true);
	generate_dot_file("layout_initialize", LAYOUT);
	std::cout << "layout_linked_widgets" << std::endl;
	layout_linked_widgets();

	try
	{
		window_implementation::layout(*this, maximum_width, maximum_height);
	}
	catch(const layout_exception_resize_failed&)
	{

		/** @todo implement the scrollbars on the window. */

		// std::stringstream sstr;
		// sstr << __FILE__ << ":" << __LINE__ << " in function '" << __func__
		// 	 << "' found the following problem: Failed to size window;"
		// 	 << " wanted size " << get_best_size() << " available size "
		// 	 << maximum_width << ',' << maximum_height << " screen size "
		// 	 << settings::screen_width << ',' << settings::screen_height << '.';

		// throw wml_exception(_("Failed to show a dialog, "
		// 					   "which doesn't fit on the screen."),
		// 					 sstr.str());
	}

	/****** Validate click dismiss status. *****/
	if(click_dismiss_ && disable_click_dismiss()) {
		assert(click_dismiss_button);
		click_dismiss_button->set_visible(widget::visibility::visible);

		connect_signal_mouse_left_click(
				*click_dismiss_button,
				std::bind(&window::set_retval, this, retval::OK, true));

		layout_initialize(true);
		generate_dot_file("layout_initialize", LAYOUT);

		layout_linked_widgets();

		try
		{
			window_implementation::layout(
					*this, maximum_width, maximum_height);
		}
		catch(const layout_exception_resize_failed&)
		{

			/** @todo implement the scrollbars on the window. */

			std::stringstream sstr;
			sstr << __FILE__ << ":" << __LINE__ << " in function '" << __func__
				 << "' found the following problem: Failed to size window;"
				 << " wanted size " << get_best_size() << " available size "
				 << maximum_width << ',' << maximum_height << " screen size "
				 << settings::screen_width << ',' << settings::screen_height
				 << '.';
			std::cout<<sstr.str()<<std::endl;
			// throw wml_exception(_("Failed to show a dialog, "
			// 					   "which doesn't fit on the screen."),
			// 					 sstr.str());
		}
	}

	/***** Get the best location for the window *****/
	size = get_best_size();
	/* Although 0-size windows might not seem valid/useful, there are
	   a handful of windows that request 0 size just to get a position
	   chosen via the code below, so at least for now allow them:
	*/
	// assert(size.x >= 0 && static_cast<unsigned>(size.x) <= maximum_width
	//        && size.y >= 0 && static_cast<unsigned>(size.y) <= maximum_height);

	point origin(0, 0);

	if(automatic_placement_) {

		switch(horizontal_placement_) {
			case grid::HORIZONTAL_ALIGN_LEFT:
				// Do nothing
				break;
			case grid::HORIZONTAL_ALIGN_CENTER:
				origin.x = (settings::screen_width - size.x) / 2;
				break;
			case grid::HORIZONTAL_ALIGN_RIGHT:
				origin.x = settings::screen_width - size.x;
				break;
			default:
				assert(false);
		}
		switch(vertical_placement_) {
			case grid::VERTICAL_ALIGN_TOP:
				// Do nothing
				break;
			case grid::VERTICAL_ALIGN_CENTER:
				origin.y = (settings::screen_height - size.y) / 2;
				break;
			case grid::VERTICAL_ALIGN_BOTTOM:
				origin.y = settings::screen_height - size.y;
				break;
			default:
				assert(false);
		}
	} else {

		// variables_.add("window_width", wfl::variant(size.x));
		// variables_.add("window_height", wfl::variant(size.y));

		// while(reevaluate_best_size_(variables_, &functions_)) {
		// 	layout_initialize(true);

		// 	window_implementation::layout(*this,
		// 								   w_(variables_, &functions_),
		// 								   h_(variables_, &functions_));

		// 	size = get_best_size();
		// 	variables_.add("window_width", wfl::variant(size.x));
		// 	variables_.add("window_height", wfl::variant(size.y));
		// }

		// variables_.add("size_request_mode", wfl::variant("size"));

		// size.x = w_(variables_, &functions_);
		// size.y = h_(variables_, &functions_);
		size.x = 300;
		size.y = 200;

		// variables_.add("window_width", wfl::variant(size.x));
		// variables_.add("window_height", wfl::variant(size.y));

		// origin.x = x_(variables_, &functions_);
		// origin.y = y_(variables_, &functions_);
		origin.x = 300;
		origin.y = 200;
	}

	/***** Set the window size *****/
	place(origin, size);

	generate_dot_file("layout_finished", LAYOUT);
	need_layout_ = false;

	event::init_mouse_location();
}

void window::layout_linked_widgets()
{
	// evaluate the group sizes
	for(auto & linked_size : linked_size_)
	{

		point max_size(0, 0);

		// Determine the maximum size.
		for(auto widget : linked_size.second.widgets)
		{

			const point size = widget->get_best_size();

			if(size.x > max_size.x) {
				max_size.x = size.x;
			}
			if(size.y > max_size.y) {
				max_size.y = size.y;
			}
		}
		if(linked_size.second.width != -1) {
			linked_size.second.width = max_size.x;
		}
		if(linked_size.second.height != -1) {
			linked_size.second.height = max_size.y;
		}

		// Set the maximum size.
		for(auto widget : linked_size.second.widgets)
		{

			point size = widget->get_best_size();

			if(linked_size.second.width != -1) {
				size.x = max_size.x;
			}
			if(linked_size.second.height != -1) {
				size.y = max_size.y;
			}

			widget->set_layout_size(size);
		}
	}
}

bool window::click_dismiss(const int mouse_button_mask)
{
	if(does_click_dismiss()) {
		if((mouse_button_state_ & mouse_button_mask) == 0) {
			set_retval(retval::OK);
		} else {
			mouse_button_state_ &= ~mouse_button_mask;
		}
		return true;
	}
	return false;
}

void window::finalize(const builder_grid& content_grid)
{
	auto widget = content_grid.build();
	assert(widget);

	static const std::string id = "_window_content_grid";

	// Make sure the new child has same id.
	widget->set_id(id);

	// auto* parent_grid = find_widget<grid>(&get_grid(), id, true, false);
	// assert(parent_grid);

	// if(grid* grandparent_grid = dynamic_cast<grid*>(parent_grid->parent())) {
	// 	grandparent_grid->swap_child(id, std::move(widget), false);
	// } else if(container_base* c = dynamic_cast<container_base*>(parent_grid->parent())) {
	// 	c->get_grid().swap_child(id, std::move(widget), true);
	// } else {
	// 	assert(false);
	// }
}

#ifdef DEBUG_WINDOW_LAYOUT_GRAPHS

void window::generate_dot_file(const std::string& generator,
								const unsigned domain)
{
	debug_layout_->generate_dot_file(generator, domain);
}
#endif

void window_implementation::layout(window& window,
									const unsigned maximum_width,
									const unsigned maximum_height)
{
	// log_scope2(log_gui_layout, LOG_IMPL_SCOPE_HEADER);

	/*
	 * For now we return the status, need to test later whether this can
	 * entirely be converted to an exception based system as in 'promised' on
	 * the algorithm page.
	 */

	try
	{
		point size = window.get_best_size();

		// DBG_GUI_L << LOG_IMPL_HEADER << " best size : " << size
		// 		  << " maximum size : " << maximum_width << ','
		// 		  << maximum_height << ".";
		if(size.x <= static_cast<int>(maximum_width)
		   && size.y <= static_cast<int>(maximum_height)) {

			// DBG_GUI_L << LOG_IMPL_HEADER << " Result: Fits, nothing to do.";
			return;
		}

		if(size.x > static_cast<int>(maximum_width)) {
			window.reduce_width(maximum_width);

			size = window.get_best_size();
			if(size.x > static_cast<int>(maximum_width)) {
				// DBG_GUI_L << LOG_IMPL_HEADER << " Result: Resize width failed."
				// 		  << " Wanted width " << maximum_width
				// 		  << " resulting width " << size.x << ".";
				throw layout_exception_width_resize_failed();
			}
			// DBG_GUI_L << LOG_IMPL_HEADER
			// 		  << " Status: Resize width succeeded.";
		}

		if(size.y > static_cast<int>(maximum_height)) {
			window.reduce_height(maximum_height);

			size = window.get_best_size();
			if(size.y > static_cast<int>(maximum_height)) {
				// DBG_GUI_L << LOG_IMPL_HEADER << " Result: Resize height failed."
				// 		  << " Wanted height " << maximum_height
				// 		  << " resulting height " << size.y << ".";
				throw layout_exception_height_resize_failed();
			}
			// DBG_GUI_L << LOG_IMPL_HEADER
			// 		  << " Status: Resize height succeeded.";
		}

		assert(size.x <= static_cast<int>(maximum_width)
			   && size.y <= static_cast<int>(maximum_height));


		// DBG_GUI_L << LOG_IMPL_HEADER << " Result: Resizing succeeded.";
		return;
	}
	catch(const layout_exception_width_modified&)
	{
		// DBG_GUI_L << LOG_IMPL_HEADER
		// 		  << " Status: Width has been modified, rerun.";

		window.layout_initialize(false);
		window.layout_linked_widgets();
		layout(window, maximum_width, maximum_height);
		return;
	}
}

void window::mouse_capture(const bool capture)
{
	// assert(event_distributor_);
	// event_distributor_->capture_mouse(capture);
}

void window::keyboard_capture(widget* widget)
{
	// assert(event_distributor_);
	// event_distributor_->keyboard_capture(widget);
}

void window::add_to_keyboard_chain(widget* widget)
{
	// assert(event_distributor_);
	// event_distributor_->keyboard_add_to_chain(widget);
}

void window::remove_from_keyboard_chain(widget* widget)
{
	// assert(event_distributor_);
	// event_distributor_->keyboard_remove_from_chain(widget);
}

void window::add_to_tab_order(widget* widget, int at)
{
	if(std::find(tab_order.begin(), tab_order.end(), widget) != tab_order.end()) {
		return;
	}
	// assert(event_distributor_);
	// if(tab_order.empty() && !event_distributor_->keyboard_focus()) {
	// 	keyboard_capture(widget);
	// }
	if(at < 0 || at >= static_cast<int>(tab_order.size())) {
		tab_order.push_back(widget);
	} else {
		tab_order.insert(tab_order.begin() + at, widget);
	}
}

// void window::signal_handler_sdl_video_resize(const event::ui_event event,
// 											  bool& handled,
// 											  const point& new_size)
// {
// 	DBG_GUI_E << LOG_HEADER << ' ' << event << ".";

// 	settings::gamemap_width += new_size.x - settings::screen_width;
// 	settings::gamemap_height += new_size.y - settings::screen_height;
// 	settings::screen_width = new_size.x;
// 	settings::screen_height = new_size.y;
// 	invalidate_layout();

// 	handled = true;
// }

void window::signal_handler_click_dismiss(const event::ui_event event,
										   bool& handled,
										   bool& halt,
										   const int mouse_button_mask)
{
	// DBG_GUI_E << LOG_HEADER << ' ' << event << " mouse_button_mask "
	// 		  << static_cast<unsigned>(mouse_button_mask) << ".";
	std::cout << LOG_HEADER << ' ' << event << " mouse_button_mask "
			  << static_cast<unsigned>(mouse_button_mask) << "." << std::endl;
	handled = halt = click_dismiss(mouse_button_mask);
}

// static bool is_active(const widget* wgt)
// {
// 	if(const styled_widget* control = dynamic_cast<const styled_widget*>(wgt)) {
// 		return control->get_active() && control->get_visible() == widget::visibility::visible;
// 	}
// 	return false;
// }

// void window::signal_handler_sdl_key_down(const event::ui_event event,
// 										  bool& handled,
// 										  const SDL_Keycode key,
// 										  const SDL_Keymod mod,
// 										  bool handle_tab)
// {
// 	DBG_GUI_E << LOG_HEADER << ' ' << event << ".";

// 	if(text_box_base* tb = dynamic_cast<text_box_base*>(event_distributor_->keyboard_focus())) {
// 		if(tb->is_composing()) {
// 			if(handle_tab && !tab_order.empty() && key == SDLK_TAB) {
// 				tb->interrupt_composition();
// 			} else {
// 				return;
// 			}
// 		}
// 	}
// 	if(key == SDLK_KP_ENTER || key == SDLK_RETURN) {
// 		if (mod & (KMOD_CTRL | KMOD_ALT | KMOD_GUI | KMOD_SHIFT)) {
// 			// Don't handle if modifier is pressed
// 			handled = false;
// 		} else {
// 			// Trigger window OK button only if Enter enabled,
// 			// otherwise pass handling to widget
// 			if (!enter_disabled_) {
// 				set_retval(retval::OK);
// 				handled = true;
// 			}
// 		}
// 	} else if(key == SDLK_ESCAPE && !escape_disabled_) {
// 		set_retval(retval::CANCEL);
// 		handled = true;
// 	} else if(key == SDLK_SPACE) {
// 		handled = click_dismiss(0);
// 	} else if(handle_tab && !tab_order.empty() && key == SDLK_TAB) {
// 		assert(event_distributor_);
// 		widget* focus = event_distributor_->keyboard_focus();
// 		auto iter = std::find(tab_order.begin(), tab_order.end(), focus);
// 		do {
// 			if(mod & KMOD_SHIFT) {
// 				if(iter == tab_order.begin()) {
// 					iter = tab_order.end();
// 				}
// 				iter--;
// 			} else {
// 				if(iter == tab_order.end()) {
// 					iter = tab_order.begin();
// 				} else {
// 					iter++;
// 					if(iter == tab_order.end()) {
// 						iter = tab_order.begin();
// 					}
// 				}
// 			}
// 		} while(!is_active(*iter));
// 		keyboard_capture(*iter);
// 		handled = true;
// 	}
// #ifdef DEBUG_WINDOW_LAYOUT_GRAPHS
// 	if(key == SDLK_F12) {
// 		debug_layout_->generate_dot_file("manual", debug_layout_graph::MANUAL);
// 		handled = true;
// 	}
// #endif
// }

// void window::signal_handler_message_show_tooltip(const event::ui_event event,
// 												  bool& handled,
// 												  const event::message& message)
// {
// 	DBG_GUI_E << LOG_HEADER << ' ' << event << ".";

// 	const event::message_show_tooltip& request
// 			= dynamic_cast<const event::message_show_tooltip&>(message);

// 	dialogs::tip::show(tooltip_.id, request.message, request.location, request.source_rect);

// 	handled = true;
// }

// void window::signal_handler_message_show_helptip(const event::ui_event event,
// 												  bool& handled,
// 												  const event::message& message)
// {
// 	DBG_GUI_E << LOG_HEADER << ' ' << event << ".";

// 	const event::message_show_helptip& request
// 			= dynamic_cast<const event::message_show_helptip&>(message);

// 	dialogs::tip::show(helptip_.id, request.message, request.location, request.source_rect);

// 	handled = true;
// }

// void window::signal_handler_request_placement(const event::ui_event event,
// 											   bool& handled)
// {
// 	// DBG_GUI_E << LOG_HEADER << ' ' << event << ".";
//     std::cout << LOG_HEADER << ' ' << event << "." << std::endl;
// 	invalidate_layout();

// 	handled = true;
// }

void window::signal_handler_close_window()
{
	set_retval(retval::AUTO_CLOSE);
}

// }---------- DEFINITION ---------{

// window_definition::window_definition(const config& cfg)
// 	: styled_widget_definition(cfg)
// {
// 	std::cout << "Parsing window cfg" << id << std::endl;

// 	//load_resolutions<resolution>(cfg);
// }

window_definition::window_definition(const resolution_definition& resol_def)
    : styled_widget_definition(resol_def)
{
	// DBG_GUI_P << "Parsing window " << id;
    std::cout << "Parsing window " << id << std::endl;
	// load_resolutions(resol_def);
	load_resolutions<resolution>(resol_def);
}

// window_definition::resolution::resolution(const config& cfg)
// 	: panel_definition::resolution(cfg), grid(nullptr)
// {
// 	// auto child = cfg.optional_child("grid");
// 	// // VALIDATE(child, _("No grid defined."));

// 	// /** @todo Evaluate whether the grid should become mandatory. */
// 	// if(child) {
// 	// 	grid = std::make_shared<builder_grid>(*child);
// 	// }
// }

window_definition::resolution::resolution(const resolution_definition& resol_def)
	: panel_definition::resolution(resol_def)
    , grid(nullptr)
{
	std::cout << "window_definition init! " << std::endl;
	// auto child = cfg.optional_child("grid");
	// // VALIDATE(child, _("No grid defined."));

	// /** @todo Evaluate whether the grid should become mandatory. */
	// if(child) {
	// 	grid = std::make_shared<builder_grid>(*child);
	// }
}

window_definition::resolution::resolution(const int win_w, 
                                            const int win_h,
                                            const int min_w,
                                            const int min_h,
                                            const int def_w,
                                            const int def_h,
                                            const int max_w,
                                            const int max_h)
	: panel_definition::resolution(win_w, 
                                    win_h,
                                    min_w,
                                    min_h,
                                    def_w,
                                    def_h,
                                    max_w,
                                    max_h)
    , grid(nullptr)
{
	std::cout << "window_definition init! " << std::endl;
	// auto child = cfg.optional_child("grid");
	// // VALIDATE(child, _("No grid defined."));

	// /** @todo Evaluate whether the grid should become mandatory. */
	// if(child) {
	// 	grid = std::make_shared<builder_grid>(*child);
	// }
}

// }------------ END --------------

} // namespace gui2

