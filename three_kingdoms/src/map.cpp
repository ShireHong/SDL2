#include <iostream>
#include "map.h"
#include "res_path.h"
#include "window.h"

tmx_map *Map::tilemap = nullptr;

Map::Map()
{

}
Map::~Map()
{
    tmx_map_free(tilemap);
}

void Map::Load(std::string file)
{
    /* Set the callback globs in the main function */
	tmx_img_load_func = (void* (*)(const char*))Window::SDL_tex_loader;
	tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
    tilemap = tmx_load(file.c_str());
    if (!tilemap) {
		tmx_perror("Cannot load map");
	}

}

void Map::Render()
{
    tmx_map * map = tilemap;
    Window::set_color(map->backgroundcolor);
	Window::draw_all_layers(map, map->ly_head);
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
void Map::RebuildMap()
{
	std::cout<<"re"<<std::endl;
	const std::string resPath = getResourcePath("resource/image");
    mapTexture = Window::LoadTexture(resPath + "guanyu_horse.png");

}
#endif