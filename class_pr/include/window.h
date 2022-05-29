#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept> /* try throw的引用头文件 */
#include <memory> /* unique_ptr */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>

/* window class*/
class Window
{
public:
	static void Init(std::string title, int w, int h);

	static void Quit();
	static void renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip = NULL);
#if 1
	static void DrawTexture(SDL_Texture *tex, const SDL_Rect &dstRect, const SDL_Rect *clip = NULL, double angle = 0.0, 
        SDL_Point *point = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
#endif
	/*static void Draw(Image *image, const Rectf &dstRect, Recti *clip = NULL,
	    float angle = 0.0, Vector2f pivot = Vector2f(0, 0), int flip = SDL_FLIP_NONE);
	*/
	static SDL_Texture* LoadTexture(std::string file);
 
    static SDL_Texture* RenderText(std::string message, std::string fontFile, SDL_Color color, int fontSize);
  
    static SDL_Texture* SurfaceToTexture(SDL_Surface *surf);
    
    static void Clear();
    
    static void Present();
private:
	static std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> mWindow;

	static std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;

	static int ScrWidth;

	static int ScrHeight;
};

#endif