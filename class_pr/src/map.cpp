#include <vector>
#include <set>
#include <string>
#include <cmath>
#include <SDL.h>
#include "window.h"
#include "map.h"
#include "res_path.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;


Map::Map() : mapTexture(nullptr)
{

}
Map::~Map()
{

}
void Map::Draw(std::string  file)
{
	if (mapTexture == nullptr){
		RebuildMap();
	}

	std::cout<<"123"<<std::endl;
	int iW = 100, iH = 100;
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;  
    SDL_Rect clips[4];
    //Since our clips our uniform in size we can generate a list of their
    //positions using some math (the specifics of this are covered in the lesson)
    for (int i = 0; i < 4; ++i){
        clips[i].x = i / 2 * iW;
        clips[i].y = i % 2 * iH;
        clips[i].w = iW;
        clips[i].h = iH;
    }
    //Specify a default clip to start with
    int useClip = 0;  
	//Window::DrawTexture(mapTexture, dst);
	Window::renderTexture(mapTexture, x, y, &clips[useClip]);
}
void Map::RebuildMap()
{
	std::cout<<"re"<<std::endl;
	const std::string resPath = getResourcePath("resource");
    mapTexture = Window::LoadTexture(resPath + "image.png");

}
