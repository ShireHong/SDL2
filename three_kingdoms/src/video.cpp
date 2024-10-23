#include "video.h"


#include "sdl/point.h"
#include "sdl/window.h"
#include "preferences/preferences.h"
#include "sdl/texture.h"
#include <memory> /* unique_ptr */
#include <iostream>
#include <SDL2/SDL_render.h> // SDL_Texture
#include <SDL2/SDL.h>
#include <cassert>
#include <vector>


namespace
{
/** The SDL window object. Will be null only if headless_. */
std::unique_ptr<sdl::window> window;

// /** The main offscreen render target. */
texture render_texture_ = {};

// /** The current offscreen render target. */
texture current_render_target_ = {};

bool headless_ = false; /**< running with no window at all */
bool testing_ = false; /**< running unit tests */
point test_resolution_ = {1024, 768}; /**< resolution for unit tests */
int refresh_rate_ = 0;
point game_canvas_size_ = {0, 0};
int pixel_scale_ = 1;
rect input_area_ = {};

} // anon namespace

namespace video
{

// Non-public interface
void render_screen(); // exposed and used only in draw_manager.cpp

// Internal functions
static void init_window(bool hidden=false);
static void init_test_window();
static void init_fake();
static void init_test();
//static bool update_framebuffer();
static bool update_test_framebuffer();
static point draw_offset();


void init(fake type)
{
	if(SDL_WasInit(SDL_INIT_VIDEO)) {
		std::cout<<"video subsystem already initialized"<<std::endl;
	}
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		std::cout << "Could not initialize SDL_video: " << std::endl;
		//throw error("Video initialization failed");
	}

	switch(type) {
	case fake::none:
		init_window();
		break;
	case fake::no_window:
		init_fake();
		break;
	case fake::no_draw:
		init_test();
		break;
	case fake::hide_window:
		init_window(true);
		break;
	default:
		std::cout<<"unrecognized fake type passed to video::init"<<std::endl;
	}
}

void deinit()
{
	//LOG_DP << "deinitializing video";

	// SDL_INIT_TIMER is always initialized at program start.
	// If it is not initialized here, there is a problem.
	assert(SDL_WasInit(SDL_INIT_TIMER));

	// Clear any static texture caches,
	// lest they try to delete textures after SDL_Quit.
	// image::flush_cache();
	// font::flush_texture_cache();
	// render_texture_.reset();
	// current_render_target_.reset();
	// gui::menu::bluebg_style.unload_images();

	// Destroy the window, and thus also the renderer.
	window.reset();

	// Close the video subsystem.
	if(SDL_WasInit(SDL_INIT_VIDEO)) {
		//LOG_DP << "quitting SDL video subsystem";
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
	if(SDL_WasInit(SDL_INIT_VIDEO)) {
		// This should not have been initialized multiple times
		std::cout<<"video subsystem still initialized after deinit"<<std::endl;
	}
}

bool headless()
{
	return headless_;
}

bool testing()
{
	return testing_;
}

void init_fake()
{
	//LOG_DP << "running headless";
	headless_ = true;
	refresh_rate_ = 1;
	game_canvas_size_ = {800,600};
}

void init_test()
{
	testing_ = true;
	refresh_rate_ = 1;
	init_test_window();
}

/** Returns true if the buffer was changed */
bool update_test_framebuffer()
{
	if (!window) {
		throw("trying to update test framebuffer with no window");
	}

	bool changed = false;

	// TODO: code unduplication
	// Build or update the current render texture.
	// if (render_texture_) {
	// 	int w, h;
	// 	SDL_QueryTexture(render_texture_, nullptr, nullptr, &w, &h);
	// 	if (w != test_resolution_.x || h != test_resolution_.y) {
	// 		// Delete it and let it be recreated.
	// 		LOG_DP << "destroying old render texture";
	// 		render_texture_.reset();
	// 	}
	// }
	// if (!render_texture_) {
	// 	//LOG_DP << "creating offscreen render texture";
	// 	render_texture_.assign(SDL_CreateTexture(
	// 		*window,
	// 		window->pixel_format(),
	// 		SDL_TEXTUREACCESS_TARGET,
	// 		test_resolution_.x, test_resolution_.y
	// 	));
	// 	LOG_DP << "updated render target to " << test_resolution_.x
	// 		<< "x" << test_resolution_.y;
	// 	changed = true;
	// }

	pixel_scale_ = 1;
	game_canvas_size_ = test_resolution_;
	input_area_ = {{}, test_resolution_};

	// The render texture is always the render target in this case.
	//force_render_target(render_texture_);

	return changed;
}
#if 1
bool update_framebuffer()
{
	if (!window) {
		//throw error("trying to update framebuffer with no window");
	}

	if (testing_) {
		return update_test_framebuffer();
	}

	bool changed = false;

	// Make sure we're getting values from the native window.
	SDL_SetRenderTarget(*window, nullptr);

	// Non-integer scales are not currently supported.
	// This option makes things neater when window size is not a perfect
	// multiple of logical size, which can happen when manually resizing.
	SDL_RenderSetIntegerScale(*window, SDL_TRUE);

	// Find max valid pixel scale at current output size.
	point osize(window->get_output_size());
    std::cout<<osize<<std::endl;
	int max_scale = std::min(
		osize.x / pref_constants::min_window_width,
		osize.y / pref_constants::min_window_height);
	max_scale = std::min(max_scale, pref_constants::max_pixel_scale);
    std::cout<<max_scale<<std::endl;
	// Determine best pixel scale according to preference and window size
	int scale = 1;
	if (prefs::get().auto_pixel_scale()) {
		// Try to match the default size (1280x720) but do not reduce below
		int def_scale = std::min(
			osize.x / pref_constants::def_window_width,
			osize.y / pref_constants::def_window_height);
		scale = std::min(max_scale, def_scale);
		// Otherwise reduce to keep below the max window size (1920x1080).
		int min_scale = std::min(
			osize.x / (pref_constants::max_window_width+1) + 1,
			osize.y / (pref_constants::max_window_height+1) + 1);
		scale = std::max(scale, min_scale);
	} else {
		scale = std::min(max_scale, prefs::get().pixel_scale());
	}
	// Cache it for easy access.
	if (pixel_scale_ != scale) {
		pixel_scale_ = scale;
		changed = true;
	}

	// // Update logical size if it doesn't match the current resolution and scale.
	 point lsize(window->get_logical_size());
	 point wsize(window->get_size());
     std::cout<<lsize<<std::endl;
     std::cout<<wsize<<std::endl;
	if (lsize.x != osize.x / scale || lsize.y != osize.y / scale) 
    {
		if (!prefs::get().auto_pixel_scale() && scale < prefs::get().pixel_scale()) 
        {
			std::cout << "reducing pixel scale from desired "
				<< prefs::get().pixel_scale() << " to maximum allowable "
				<< scale;
		}
		std::cout << "pixel scale: " << scale <<std::endl;
		std::cout << "overriding logical size"<<std::endl;
		std::cout << "  old lsize: " << lsize <<std::endl;
		std::cout << "  old wsize: " << wsize <<std::endl;
		std::cout << "  old osize: " << osize <<std::endl;
		window->set_logical_size(osize.x / scale, osize.y / scale);
		lsize = window->get_logical_size();
		wsize = window->get_size();
		osize = window->get_output_size();
		std::cout << "  new lsize: " << lsize <<std::endl;
		std::cout << "  new wsize: " << wsize <<std::endl;
		std::cout << "  new osize: " << osize <<std::endl;
		float sx, sy;
		SDL_RenderGetScale(*window, &sx, &sy);
		std::cout << "  render scale: " << sx << ", " << sy <<std::endl;
	}
	// Cache it for easy access
	game_canvas_size_ = lsize;

	// Build or update the current render texture.
	if (render_texture_) {
		int w, h;
		SDL_QueryTexture(render_texture_, nullptr, nullptr, &w, &h);
		if (w != osize.x || h != osize.y) {
			// Delete it and let it be recreated.
			std::cout << "destroying old render texture" << std::endl;
			render_texture_.reset();
		} else {
			// This isn't currently used, but ensure it's accurate anyway.
			render_texture_.set_draw_size(lsize);
		}
	}
	if (!render_texture_) {
		//LOG_DP << "creating offscreen render texture";
        std::cout << "creating offscreen render texture" << std::endl;
		render_texture_.assign(SDL_CreateTexture(
			*window,
			window->pixel_format(),
			SDL_TEXTUREACCESS_TARGET,
			osize.x, osize.y
		));
		// This isn't really necessary, but might be nice to have attached
		render_texture_.set_draw_size(lsize);
		changed = true;
	}

	// Assign the render texture now. It will be used for all drawing.
	force_render_target(render_texture_);

	// By default input area is the same as the window area.
	input_area_ = {{}, wsize};

	rect active_area = to_output(draw_area());

    std::cout << "active_area: " << active_area << std::endl;
	if (active_area.size() != osize) {
		// LOG_DP << "render target offset: LT " << active_area.pos() << " RB "
		//        << osize - active_area.size() - active_area.pos();
        std::cout << "render target offset: LT " << active_area.pos() << " RB "
		        << osize - active_area.size() - active_area.pos() << std::endl;
		// Translate active_area into display coordinates as input_area_
		input_area_ = {
			(active_area.pos() * wsize) / osize,
			(active_area.size() * wsize) / osize
		};
		//LOG_DP << "input area: " << input_area_;
        std::cout << "input area: " << input_area_ << std::endl;
	}

	return changed;
}
#endif
void init_test_window()
{
	// LOG_DP << "creating test window " << test_resolution_.x
	// 	<< "x" << test_resolution_.y;

	uint32_t window_flags = 0;
	window_flags |= SDL_WINDOW_HIDDEN;
	// The actual window won't be used, as we'll be rendering to texture.

	uint32_t renderer_flags = 0;
	renderer_flags |= SDL_RENDERER_TARGETTEXTURE;
	// All we need is to be able to render to texture.

	window.reset(new sdl::window(
		"", 0, 0, test_resolution_.x, test_resolution_.y,
		window_flags, renderer_flags
	));

	update_test_framebuffer();
}

void init_window(bool hidden)
{
    const int x = SDL_WINDOWPOS_CENTERED;
	const int y = SDL_WINDOWPOS_CENTERED;
	const point res = {200,300};
	const int w = res.x;
	const int h = res.y;

	uint32_t window_flags = 0;

	// Add any more default flags here
	window_flags |= SDL_WINDOW_RESIZABLE;
	window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;

	uint32_t renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;



	// Initialize window
	window.reset(new sdl::window("", x, y, w, h, window_flags, renderer_flags));

	// It is assumed that this function is only ever called once.
	// If that is no longer true, then you should clean things up.
	
    //assert(!render_texture_);



	SDL_DisplayMode currentDisplayMode;
	SDL_GetCurrentDisplayMode(window->get_display_index(), &currentDisplayMode);
	refresh_rate_ = currentDisplayMode.refresh_rate != 0 ? currentDisplayMode.refresh_rate : 60;

	update_framebuffer();
}

bool has_window()
{
	return bool(window);
}

point output_size()
{
	if (testing_) {
		return test_resolution_;
	}
	// As we are rendering via an abstraction, we should never need this.
	return window->get_output_size();
}

point window_size()
{
	if (testing_) {
		return test_resolution_;
	}
	return window->get_size();
}

rect game_canvas()
{
	return {0, 0, game_canvas_size_.x, game_canvas_size_.y};
}

point game_canvas_size()
{
	return game_canvas_size_;
}

rect draw_area()
{
	return {0, 0, current_render_target_.w(), current_render_target_.h()};
}


point draw_offset()
{
	// As we are using SDL_RenderSetIntegerScale, there may be a slight
	// offset of the drawable area on the render target if the target size
	// is not perfectly divisble by the scale.
	// SDL doesn't provide any way of retrieving this offset,
	// so we just have to base our calculation on the known behaviour.
	point osize = output_size();
	point dsize = {200,200}/*draw_size()*/;
	point scale = osize / dsize;
	return (osize - (scale * dsize)) / 2;
}

rect output_area()
{
	point p = output_size();
	return {0, 0, p.x, p.y};
}

rect to_output(const rect& r)
{
	// Multiply r by integer scale, adding draw_offset to the position.
	point dsize = current_render_target_.draw_size();
	point osize = current_render_target_.get_raw_size();
	point pos = (r.pos() * (osize / dsize)) + draw_offset();
	point size = r.size() * (osize / dsize);
	return {pos, size};
}

rect input_area()
{
	return input_area_;
}

int get_pixel_scale()
{
	return pixel_scale_;
}

int current_refresh_rate()
{
	// TODO: this should be more clever, depending on usage
	return refresh_rate_;
}


void force_render_target(const texture& t)
{
	if (SDL_SetRenderTarget(get_renderer(), t)) {
		// ERR_DP << "failed to set render target to "
		// 	<< static_cast<void*>(t.get()) << ' '
		// 	<< t.draw_size() << " / " << t.get_raw_size();
		// ERR_DP << "last SDL error: " << SDL_GetError();
        std::cout << "failed to set render target to "
		 	<< static_cast<void*>(t.get()) << ' '
		 	<< t.draw_size() << " / " << t.get_raw_size() << std::endl;
		std::cout << "last SDL error: " <<  std::endl;
		//throw error("failed to set render target");
	}
	current_render_target_ = t;

	if (testing_) {
		return;
	}

	// The scale factor gets reset when the render target changes,
	// so make sure it gets set back appropriately.
	if (!t) {
		// DBG_DP << "rendering to window / screen";
        std::cout << "rendering to window / screen" << std::endl;
		window->set_logical_size(game_canvas_size_);
	} else if (t == render_texture_) {
		//DBG_DP << "rendering to primary buffer";
        std::cout << "rendering to primary buffer" << std::endl;
		window->set_logical_size(game_canvas_size_);

        std::cout << "game_canvas_size_: " << game_canvas_size_ << std::endl;
	} else {
		// DBG_DP << "rendering to custom target "
		// 	<< static_cast<void*>(t.get()) << ' '
		// 	<< t.draw_size() << " / " << t.get_raw_size();
        std::cout << "rendering to custom target "
		 	<< static_cast<void*>(t.get()) << ' '
		 	<< t.draw_size() << " / " << t.get_raw_size() << std::endl;
		window->set_logical_size(t.w(), t.h());
	}
}


void set_window_title(const std::string& title)
{
	assert(window);
	window->set_title(title);
}

void set_window_icon(SDL_Surface* icon)
{
	assert(window);
	window->set_icon(icon);
}

SDL_Renderer* get_renderer()
{
	if(window) {
		return *window;
	} else {
		return nullptr;
	}
}

SDL_Window* get_window()
{
	return *window;
}

std::string current_driver()
{
	const char* const drvname = SDL_GetCurrentVideoDriver();
	return drvname ? drvname : "<not initialized>";
}

std::vector<std::string> enumerate_drivers()
{
	std::vector<std::string> res;
	int num_drivers = SDL_GetNumVideoDrivers();

	for(int n = 0; n < num_drivers; ++n) {
		const char* drvname = SDL_GetVideoDriver(n);
		res.emplace_back(drvname ? drvname : "<invalid driver>");
	}

	return res;
}

/**
 * Tests whether the given flags are currently set on the SDL window.
 *
 * @param flags               The flags to test, OR'd together.
 */
static bool window_has_flags(uint32_t flags)
{
	return window && (window->get_flags() & flags) != 0;
}

bool window_is_visible()
{
	return window_has_flags(SDL_WINDOW_SHOWN);
}

bool window_has_focus()
{
	return window_has_flags(SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
}

bool window_has_mouse_focus()
{
	return window_has_flags(SDL_WINDOW_MOUSE_FOCUS);
}

} // namespace video
