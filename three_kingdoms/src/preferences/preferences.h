
#pragma once

#include <iostream>
#include <set>

struct point;
class unit_map;
class game_board;

namespace pref_constants
{
const int min_window_width  = 800;
const int min_window_height = 540;

const int def_window_width  = 1280;
const int def_window_height = 720;

const int max_window_width = 1920;
const int max_window_height = 1080;

const int min_font_scaling  = 80;
const int max_font_scaling  = 150;

const int min_pixel_scale = 1;
const int max_pixel_scale = 4;

const int TRANSITION_UPDATE_OFF = 0;
const int TRANSITION_UPDATE_ON = 1;
const int TRANSITION_UPDATE_PARTIAL = 2;
const int TRANSITION_UPDATE_COUNT = 3;

const unsigned char CREDENTIAL_SEPARATOR = '\f';
const std::string EMPTY_LOGIN = "@@";

const int INFINITE_AUTO_SAVES = 61;

const std::string default_addons_server = "add-ons.wesnoth.org";

// preferences for MP alerts
// all listed here have three variants with the suffixes: _lobby, _sound, _notif
const std::string player_joins = "player_joins";
const std::string player_leaves = "player_leaves";
const std::string private_message = "private_message";
const std::string friend_message = "friend_message";
const std::string public_message = "public_message";
const std::string server_message = "server_message";
const std::string ready_for_start = "ready_for_start";
const std::string game_has_begun = "game_has_begun";
const std::string turn_changed = "turn_changed";
const std::string game_created = "game_created";

enum class lobby_joins { show_none, show_friends, show_all };

enum PREFERENCE_VIEW { VIEW_DEFAULT, VIEW_FRIENDS };
}

class prefs
{
	friend struct preferences_dialog_friend;
	// friend int impl_preferences_set(lua_State* L);
	// friend int impl_preferences_get(lua_State* L);

	public:
		static prefs& get()
		{
			// for wesnoth: checks that the userdata folders have been initialized/found since if it hasn't been then it's too soon to be getting any values from it
			// for boost: the userdata folders don't get initialized and the preferences aren't used for anything, so skip the check here
			//
			// nice idea, but it seems that literally the only thing that initializes the userdata folders before something ends up calling the preferences is running wesnoth normally
			// and that's not practical to check for
			//static bool called_before_init = !(filesystem::base_name(filesystem::get_exe_path()).find("boost") == std::string::npos && !filesystem::is_userdata_initialized());
			//assert(called_before_init);

			static prefs prefs_manager;
			return prefs_manager;
		}

		~prefs();

		bool auto_pixel_scale();
		void set_auto_pixel_scale(bool choice);

        int pixel_scale();
        void set_pixel_scale(const int scale);
    private:
		prefs();
		// don't move, assign, or copy a singleton
		prefs(const prefs& p) = delete;
		prefs& operator=(const prefs& p) = delete;
		prefs(const prefs&& p) = delete;
		prefs& operator=(const prefs&& p) = delete;
};
