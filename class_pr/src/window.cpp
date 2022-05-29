#include <SDL.h>
#include <SDL_image.h>
#include "window.h"

using namespace std;

unique_ptr<SDL_Window, void (*)(SDL_Window*)> Window::mWindow 
        = unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr,SDL_DestroyWindow);
unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Window::mRenderer 
        = unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr,SDL_DestroyRenderer);

int Window::ScrWidth;
int Window::ScrHeight;

void Window::Init(string title, int w, int h)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        throw runtime_error("SDL Init Failed");
    if(TTF_Init() == -1)
        throw runtime_error("TTF Init Failed");

    ScrWidth = w;
    ScrHeight = h;
    mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        ScrWidth, ScrHeight, SDL_WINDOW_SHOWN));
    if(mWindow == nullptr)
        throw runtime_error("Fail to open window");

    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (mRenderer == nullptr)
        throw std::runtime_error("Failed to start renderer");
}

void Window::Quit()
{
    //Release the window and renderer
    mRenderer.~unique_ptr();
    mWindow.~unique_ptr();
    TTF_Quit();
    SDL_Quit();
}

void Window::renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != NULL){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    SDL_RenderCopy(mRenderer.get(), tex, clip, &dst);
}

#if 1
void Window::DrawTexture(SDL_Texture *tex, const SDL_Rect &dstRect, const SDL_Rect *clip , double angle, 
        SDL_Point *point, SDL_RendererFlip flip)
{
    //Calculate the pivot point as an offset from image center
    point->x += dstRect.w / 2;
    point->y += dstRect.h / 2;
    //Make sure we don't try to convert a NULL clip rect
    
    //Draw the texture
    SDL_RenderCopyEx(mRenderer.get(), tex, clip, &dstRect, angle, point, flip);
}
#endif
SDL_Texture* Window::LoadTexture(std::string file){
    SDL_Texture *tex = nullptr;
    tex = IMG_LoadTexture(mRenderer.get(), file.c_str());
    return tex;
}
SDL_Texture* Window::RenderText(std::string message, std::string fontFile, 
                                SDL_Color color, int fontSize)
{
    //Open the font
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    
    //Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
    //Clean up unneeded stuff
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);

    return texture;
}
SDL_Texture* Window::SurfaceToTexture(SDL_Surface *surf){
    SDL_Texture *tex = nullptr;
    tex = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
    SDL_FreeSurface(surf);
    return tex;
}

void Window::Clear(){
    SDL_RenderClear(mRenderer.get());
}
void Window::Present(){
    SDL_RenderPresent(mRenderer.get());
}
