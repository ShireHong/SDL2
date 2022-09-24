#include <vector>
#include <set>
#include <string>
#include "map.h"
#include "res_path.h"
#include "window.h"
#include "camera.h"
#include "cleanup.h"


Map::Map() : mapTexture(nullptr)
{
}
Map::~Map()
{
    cleanup(mapTexture);
    tmx_map_free(tilemap);
}

void Map::Load(std::string file)
{
    /* Set the callback globs in the main function */
	tmx_img_load_func = (void* (*)(const char*))Window::SDL_tex_loader;
    tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
#if 0
    tilemap = tmx_load(file.c_str());
    if (!tilemap) {
		tmx_perror("Cannot load map");
	}
#endif
    

    //Set the box accordingly
	mBox.Set(0, 0, 960, 720);
}

void Map::Render()
{
    tmx_map * map = tilemap;
    Window::set_color(map->backgroundcolor);
	Window::draw_all_layers(map, map->ly_head);
}
//void Map::Draw(std::string  file)
void Map::Draw(std::weak_ptr<Camera> cam)
{
	if (mapTexture == nullptr)
    {
		RebuildMap();
        
	}
     
	auto cameraShared = cam.lock();

    Rectf dst = {92, 300, 1052, 1020};
    Recti clip = {0, 0, 960, 720};
    
    Window::DrawTexture_ex(mapTexture, dst, &clip);



	//Rectf pos(0, cameraShared->Centering().y, 960, 720);
    //std::cout<<cameraShared->Centering().x<<" "<<cameraShared->Centering().y<<" "<<cameraShared->Box().w<<" "<<cameraShared->Box().h<<std::endl;
    //Rectf pos(0, 0, 960, 720);
	//Window::DrawTexture(mapTexture, pos);
    //Window::renderTexture(mapTexture, 0, 0, NULL);
}

Recti Map::Box() const 
{
	return mBox;
}
#if 0
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
#endif
void Map::RebuildMap()
{
	mapTexture = Window::LoadTexture("./resource/tiles/three_kingdom.png");
    SDL_Rect clip = {0, 0, 960, 720};
    //Window::renderTexture(mapTexture, 0, 0, &clip);
    
}
