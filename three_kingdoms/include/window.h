#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept> /* try throw的引用头文件 */
#include <memory> /* unique_ptr */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <tmx.h>
#include "timer.h"
#include "rect.h"

/* window class*/
class Window
{
public:
	static void Init(std::string title, int w, int h);

	static void Quit();
	static void renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip = NULL);

	/*tmx draw*/
	SDL_Texture* SDL_tex_loader(const char *path);

	static void set_color(int color);

	static void draw_polyline(double **points, double x, double y, int pointsc);

	static void draw_polygon(double **points, double x, double y, int pointsc);

	static void draw_objects(tmx_object_group *objgr);

	static void draw_layer(tmx_map *map, tmx_layer *layer);

	static void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags);

	static void draw_image_layer(tmx_image *image);

	static void draw_all_layers(tmx_map *map, tmx_layer *layers);

	static void DrawTexture(SDL_Texture *tex, const Rectf &dstRect, Recti *clip = NULL, float angle = 0.0, 
        					Vector2f pivot = Vector2f(0, 0), SDL_RendererFlip flip = SDL_FLIP_NONE);

	static void DrawTexture_ex(SDL_Texture *tex, SDL_Rect *dstRect, SDL_Rect *clip,
                    float angle = 0.0, const SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	/*static void Draw(Image *image, const Rectf &dstRect, Recti *clip = NULL,
	    float angle = 0.0, Vector2f pivot = Vector2f(0, 0), int flip = SDL_FLIP_NONE);
	*/
	static SDL_Texture* LoadTexture(std::string file);
 
    static SDL_Texture* RenderText(std::string message, std::string fontFile, SDL_Color color, int fontSize);
  
    static SDL_Texture* SurfaceToTexture(SDL_Surface *surf);

	static Recti Box();
    
    static void ShowAvgFps(bool log);
    
    static void Clear();
    
    static void Present();
private:
	static std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> mWindow;
	static std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;
	static int ScrWidth;
	static int ScrHeight;
	static Timer mTimer;
    static int mFrame;
    static Recti mBox;
};

#endif