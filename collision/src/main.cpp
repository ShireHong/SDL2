#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"
#include <memory>
#include "SDL_mixer.h"
#include <vector>

using namespace std;

/*
 * Lesson 5: Clipping Sprite Sheets
 */
//Screen attributes
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
//The frame rate
const int FRAMES_PER_SECOND = 20;

SDL_Window      *window = NULL;
SDL_Renderer    *renderer = NULL;


//The dot attributes
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;
//将要播放的音乐
Mix_Music *music = NULL;
 
//将要使用的声效
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;


void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr){
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderTexture(tex, ren, dst, clip);
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
        int fontSize, SDL_Renderer *renderer)
{
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        logSDLError(std::cout, "TTF_OpenFont");
        return nullptr;
    }
    //We need to first render to a surface as that's what TTF_RenderText returns, then
    //load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr){
        TTF_CloseFont(font);
        logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        logSDLError(std::cout, "CreateTexture");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}



class Object
{
private:
    int X;
    int Y;
    int Velx;
    int Vely;
    SDL_Texture *Tex;
    vector<SDL_Rect> Box;

    void UpdateBox();
public:
    Object(int x, int y, SDL_Texture *tex);

    void Input(SDL_Event e);

    void Move(vector<SDL_Rect> &rects);

    void Show();

    vector<SDL_Rect> &GetRects();

};

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void Start();
    void Stop();
    void Pause();
    void Restart();

    //Gets the timer's time
    int GetTicks();

    //Checks the status of the timer
    bool Isstarted();
    bool Ispaused();
};

Object::Object(int x, int y, SDL_Texture *tex)
{
    X = x;
    Y = y;

    Velx = 0;
    Vely = 0;

    Tex = tex;

    //Create the necessary SDL_Rects
    Box.resize( 11 );

    //Initialize the collision Boxes' width and height
    Box[ 0 ].w = 6;
    Box[ 0 ].h = 1;

    Box[ 1 ].w = 10;
    Box[ 1 ].h = 1;

    Box[ 2 ].w = 14;
    Box[ 2 ].h = 1;

    Box[ 3 ].w = 16;
    Box[ 3 ].h = 2;

    Box[ 4 ].w = 18;
    Box[ 4 ].h = 2;

    Box[ 5 ].w = 20;
    Box[ 5 ].h = 6;

    Box[ 6 ].w = 18;
    Box[ 6 ].h = 2;

    Box[ 7 ].w = 16;
    Box[ 7 ].h = 2;

    Box[ 8 ].w = 14;
    Box[ 8 ].h = 1;

    Box[ 9 ].w = 10;
    Box[ 9 ].h = 1;

    Box[ 10 ].w = 6;
    Box[ 10 ].h = 1;

    UpdateBox();

}

/*根据当前的物体坐标更新每个Box的坐标*/
void Object::UpdateBox()
{
    int size = Box.size();
    int h_init = 0;
    /*每一个坐标都是左上角位置*/
    for(int i=0; i < size;i++)
    {
        Box[i].x = X + (DOT_WIDTH - Box[i].w) / 2;
        /*累加*/
        Box[i].y = Y + h_init;

        h_init += Box[i].h;
    }
}

void Object::Input(SDL_Event e)
{
    if( e.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:       
                Vely -= 1; 
                break;
            case SDLK_DOWN:     
                Vely += 1; 
                break;
            case SDLK_LEFT:     
                Velx -= 1; 
                break;
            case SDLK_RIGHT:    
                Velx += 1; 
                break;
            default:
                break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: 
                Vely += 1; 
                break;
            case SDLK_DOWN: 
                Vely -= 1; 
                break;
            case SDLK_LEFT: 
                Velx += 1; 
                break;
            case SDLK_RIGHT: 
                Velx -= 1; 
                break;
            default:
                break;
        }
    }
    //cout<<Velx<<"------"<<Vely<<endl;
}

bool checkcollision( std::vector<SDL_Rect> &A, std::vector<SDL_Rect> &B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for( int Abox = 0; Abox < A.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = A[ Abox ].x;
        rightA = A[ Abox ].x + A[ Abox ].w;
        topA = A[ Abox ].y;
        bottomA = A[ Abox ].y + A[ Abox ].h;

        //Go through the B boxes
        for( int Bbox = 0; Bbox < B.size(); Bbox++ )
        {
            //Calculate the sides of rect B
            leftB = B[ Bbox ].x;
            rightB = B[ Bbox ].x + B[ Bbox ].w;
            topB = B[ Bbox ].y;
            bottomB = B[ Bbox ].y + B[ Bbox ].h;

            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }

    //If neither set of collision boxes touched
    return false;
}

void Object::Move(vector<SDL_Rect> &rects)
{
    X += Velx;
    Y += Vely;

    UpdateBox();

    if(X < 0 || X > SCREEN_WIDTH || checkcollision(Box, rects))
    {
        X -= Velx;
        UpdateBox();
    }

    if(Y < 0 || Y > SCREEN_WIDTH || checkcollision(Box, rects))
    {
        X -= Velx;
        UpdateBox();
    }
}

void Object::Show()
{
    renderTexture(Tex, renderer, X, Y, NULL);
}


std::vector<SDL_Rect> &Object::GetRects()
{
    //Retrieve the collision boxes
    return Box;
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::Start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::Stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::Pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::Restart()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::GetTicks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::Isstarted()
{
    return started;
}

bool Timer::Ispaused()
{
    return paused;
}


void DrawRect(SDL_Renderer * renderer, SDL_Rect *rect, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_RenderFillRect(renderer, rect);
}


int main(int, char**){
    //Start up SDL and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }
    //Also need to init SDL_ttf
    if (TTF_Init() != 0){
        logSDLError(std::cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }

    //Setup our window and renderer
    window = SDL_CreateWindow("Mixer", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }
    const std::string resPath = getResourcePath("resource");
    SDL_Texture *image = loadTexture(resPath + "image/dot.bmp", renderer);
    if (image == nullptr){
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color color = { 0, 0, 255, 100 };
    SDL_Texture *font = renderText("Hello SDL2!", resPath + "font/arial.ttf", color, 20, renderer);
    if (font == nullptr){
        cleanup(font, renderer, window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;    
    }

    string be   = resPath + "music/beat.wav";
    string sc   = resPath + "music/scratch.wav";
    string hi   = resPath + "music/high.wav";
    string me   = resPath + "music/medium.wav";
    string lo   = resPath + "music/low.wav";

    music = Mix_LoadMUS( be.c_str() );
    
    //如果加载音乐出了问题
    if( music == NULL )
    {
        return 1;    
    }
    

    //加载声效
    scratch = Mix_LoadWAV( sc.c_str() );
    high = Mix_LoadWAV( hi.c_str() );
    med = Mix_LoadWAV( me.c_str() );
    low = Mix_LoadWAV( lo.c_str() );
    
    //如果加载声效出了问题
    if( ( scratch == NULL ) || ( high == NULL ) || ( med == NULL ) || ( low == NULL ) )
    {
        return 1;    
    }

    SDL_Event e;
    bool quit = false;

    Object myDot( 0, 0, image), otherDot( 20, 20, image);

    Timer fps;

    fps.Start();

    while (!quit){
        //Event Polling
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT){
                quit = true;
            }

            myDot.Input(e);
            //Use number input to select which clip should be drawn
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_1:
                    case SDLK_KP_1:
                        //播放“挠”的声效
                        if( Mix_PlayChannel( -1, scratch, 0 ) == -1 )
                        {
                            std::cout<<"scrtch wrong"<<std::endl;    
                        }
                        break;
                    case SDLK_2:
                    case SDLK_KP_2:
                        //播放“重击”的声效
                        if( Mix_PlayChannel( -1, high, 0 ) == -1 )
                        {
                            std::cout<<"high wrong"<<std::endl;     
                        }
                        break;
                    case SDLK_3:
                    case SDLK_KP_3:
                         //播放“中度击打”的声效
                        if( Mix_PlayChannel( -1, med, 0 ) == -1 )
                        {
                            std::cout<<"high med"<<std::endl;    
                        }
                        break;
                    case SDLK_4:
                    case SDLK_KP_4:
                        //播放“轻击”的声效
                        if( Mix_PlayChannel( -1, low, 0 ) == -1 )
                        {
                            std::cout<<"high low"<<std::endl;    
                        }
                        break;
                    case SDLK_9:
                    case SDLK_KP_9:
                        //如果没有音乐在播放
                        if( Mix_PlayingMusic() == 0 )
                        {
                            //播放音乐
                            if( Mix_PlayMusic( music, -1 ) == -1 )
                            {
                                std::cout<<"play wrong"<<std::endl;
                            }    
                        }
                        //如果音乐已进入播放状态
                        else
                        {
                            //如果音乐被暂停了
                            if( Mix_PausedMusic() == 1 )
                            {
                                //恢复播放
                                Mix_ResumeMusic();
                            }
                            //如果音乐正在播放
                            else
                            {
                                //暂停音乐
                                Mix_PauseMusic();
                            }
                        }
                        break;
                    case SDLK_0:
                    case SDLK_KP_0:
                        //停止音乐
                        Mix_HaltMusic();
                        break;
                    case SDLK_ESCAPE:
                        
                        break;
                    default:
                        break;
                }
            }
        }
        //Rendering

        myDot.Move( otherDot.GetRects() );

        SDL_RenderClear(renderer);
        //Draw the image
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = 640;
        rect.h = 480;
        DrawRect(renderer, &rect, 255, 255, 255, 255);

        otherDot.Show();
        myDot.Show();
        //Update the screen

         //Cap the frame rate
        if( fps.GetTicks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.GetTicks() );
        }

        //renderTexture(font, renderer, 20, 20);
        SDL_RenderPresent(renderer);
    }
    //Clean up
    cleanup(image, renderer, window);
    cleanup(scratch, high, med, low);
    cleanup(music);
    //关闭字体
    //TTF_CloseFont(font);
    
    //退出SDL_mixer
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    return 0;
}