#include <iostream>
#include "controller_base.h"
#include "mouse_handler_base.h"
#include "events.h"


controller_base::controller_base()
        :last_mouse_is_touch_(false)
        ,long_touch_timer_(0)
{

}

controller_base::~controller_base()
{

}


void controller_base::handle_event(const SDL_Event& event)
{

	events::mouse_handler_base& mh_base = get_mouse_handler_base();

	SDL_Event new_event = {};

	switch(event.type) {
	case SDL_MOUSEMOTION:
		// Ignore old mouse motion events in the event queue
		if(SDL_PeepEvents(&new_event, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) > 0) {
			while(SDL_PeepEvents(&new_event, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION) > 0) {
			};
			if(new_event.motion.which != SDL_TOUCH_MOUSEID) {
				mh_base.mouse_motion_event(new_event.motion, false);
                
                std::cout<<"motion 1"<<std::endl;
			}
		} else {
			if(new_event.motion.which != SDL_TOUCH_MOUSEID) {
				mh_base.mouse_motion_event(event.motion, false);
                std::cout<<"motion 2"<<std::endl;
			}
            
            
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		// last_mouse_is_touch_ = event.button.which == SDL_TOUCH_MOUSEID;

		// if(last_mouse_is_touch_ && long_touch_timer_ == 0) {
		// 	long_touch_timer_ = gui2::add_timer(
		// 			long_touch_duration_ms,
		// 			std::bind(&controller_base::long_touch_callback, this, event.button.x, event.button.y));
		// }

		// mh_base.mouse_press(event.button, is_browsing());
		// hotkey::mbutton_event(event, get_hotkey_command_executor());
		break;

	case SDL_MOUSEBUTTONUP:
		// if(long_touch_timer_ != 0) {
		// 	gui2::remove_timer(long_touch_timer_);
		// 	long_touch_timer_ = 0;
		// }

		// last_mouse_is_touch_ = event.button.which == SDL_TOUCH_MOUSEID;

		// mh_base.mouse_press(event.button, is_browsing());
		// if(mh_base.get_show_menu()) {
		// 	show_menu(get_display().get_theme().context_menu()->items(), event.button.x, event.button.y, true,
		// 			get_display());
		// }
        std::cout<<"up"<<std::endl;
		break;
	// TODO: Support finger specifically, like pan the map. For now, SDL's "shadow mouse" events will do.
	case SDL_MULTIGESTURE:
	default:
		break;
	}
}

void controller_base::play_slice(bool is_delay_enabled)
{
	// CKey key;

	// if(plugins_context* l = get_plugins_context()) {
	// 	l->play_slice();
	// }

	events::pump();
	//events::raise_process_event();
	//events::draw();

	// // Update sound sources before scrolling
	// if(soundsource::manager* l = get_soundsource_man()) {
	// 	l->update();
	// }

	// const theme::menu* const m = get_display().menu_pressed();
	// if(m != nullptr) {
	// 	const rect& menu_loc = m->location(video::game_canvas());
	// 	show_menu(m->items(), menu_loc.x + 1, menu_loc.y + menu_loc.h + 1, false, get_display());

	// 	return;
	// }

	// const theme::action* const a = get_display().action_pressed();
	// if(a != nullptr) {
	// 	const rect& action_loc = a->location(video::game_canvas());
	// 	execute_action(a->items(), action_loc.x + 1, action_loc.y + action_loc.h + 1, false);

	// 	return;
	// }

	// auto str_vec = additional_actions_pressed();
	// if(!str_vec.empty()) {
	// 	execute_action(str_vec, 0, 0, false);
	// 	return;
	// }

	// bool was_scrolling = scrolling_;

	// int mousex, mousey;
	// uint8_t mouse_flags = sdl::get_mouse_state(&mousex, &mousey);

	// scrolling_ = handle_scroll(mousex, mousey, mouse_flags);

	// map_location highlighted_hex = get_display().mouseover_hex();

	// // be nice when window is not visible	// NOTE should be handled by display instead, to only disable drawing
	// if(is_delay_enabled && !video::window_is_visible()) {
	// 	SDL_Delay(200);
	// }

	// // Scrolling ended, update the cursor and the brightened hex
	// if(!scrolling_ && was_scrolling) {
	// 	get_mouse_handler_base().mouse_update(is_browsing(), highlighted_hex);
	// }
}