#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "res_path.h"
#include "cleanup.h"


const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
//We'll be scaling our tiles to be 40x40
const int TILE_SIZE = 40;

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr){
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param w The width of the texture to draw
 * @param h The height of the texture to draw
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

int main(int, char**){
    //Start up SDL and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    //Setup our window and renderer
    SDL_Window *window = SDL_CreateWindow("Img_load", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    //The textures we'll be using
    const std::string resPath = getResourcePath("resource");
    SDL_Texture *image = loadTexture(resPath + "2.png", renderer);
    SDL_Texture *player = loadTexture(resPath + "down_1.png", renderer);
    //Make sure they both loaded ok
    if ( image == nullptr){
        cleanup(image, renderer, window);
        SDL_Quit();
        return 1;
    }

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    
    //Our event unent e;
    //For tracking if we want to quit
    bool quit = false;

    SDL_Event xx;
    while (!quit)
    {
        //Read any events that occured, for now we'll just quit if any event occurs
        while (SDL_PollEvent(&xx)){
            //If user closes the window
            if (xx.type == SDL_QUIT){
                quit = true;
            }
            //If user presses any key
            if (xx.type == SDL_KEYDOWN){
                //quit = true;
            }
            //If user clicks the mouse
            if (xx.type == SDL_MOUSEBUTTONDOWN){
                //quit = true;
            }
            if (xx.type == SDL_MOUSEMOTION){
                //std::cout<<xx.motion.x<<" "<<xx.motion.y<<std::endl;
            }
            if (xx.type == SDL_MOUSEWHEEL){
                std::cout<<xx.wheel.x<<" "<<xx.wheel.y<<std::endl;
            }
        }
        //Rendering
        SDL_RenderClear(renderer);
        //Draw the image
        renderTexture(image, renderer, x, y);

        renderTexture(player, renderer, 320, 240);
        //Update the screen
        SDL_RenderPresent(renderer);
    }


    //Destroy the various items
    cleanup( image, renderer, window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}