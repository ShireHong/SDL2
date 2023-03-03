#include <vector>
#include <set>
#include <string>
#include "map.h"
#include "res_path.h"
#include "window.h"
#include "camera.h"
#include "cleanup.h"

//Tile Map::mTile;
tmx_map* Map::tilemap;

Map::Map() : mapTexture(nullptr)
{
}
Map::~Map()
{
    cleanup(mapTexture);
    tmx_map_free(tilemap);
}


void show_map_info(tmx_map *map)
{	
#if 0
    std::cout<<"map width:"<<map->width<<std::endl;
    std::cout<<"map height:"<<map->height<<std::endl;
    std::cout<<"tile width:"<<map->tile_width<<std::endl;
    std::cout<<"tile height:"<<map->tile_height<<std::endl;
    std::cout<<"tile count:"<<map->tilecount<<std::endl;

	tmx_layer *layer1 = tmx_find_layer_by_name(map,"bk");

	std::cout<<"id  "<<layer1->id<<std::endl;
    std::cout<<"map width:"<<layer1->content.image->width<<std::endl;
    std::cout<<"map height:"<<layer1->content.image->height<<std::endl;

	tmx_layer *layer2 = tmx_find_layer_by_name(map,"bottom");

	std::cout<<"id  "<<layer2->id<<std::endl;
	std::cout<<"name  "<<layer2->name<<std::endl;
	
#endif

#if 1

	tmx_layer *layer3 = tmx_find_layer_by_name(map,"bottom");

	tmx_property *prop;

	prop = tmx_get_property(layer3->properties, "coli");
	std::cout<<prop->value.integer<<std::endl;
    //int gid = 30 * player_y/map->tile_height + player_x/map->tile_width;
    #if 0
    for(int i = 0; i < 18; i++)
    {
        for(int j = 0; j < 24; j++)
        {
            std::cout<<layer3->content.gids[24*i + j]<<" ";
        }
        std::cout<<std::endl;
    }
    #endif
#endif
}

void Map::InitTileInfo()
{
    #if 0
    mTile.tile_width = tilemap->tile_width;
    mTile.tile_height = tilemap->tile_height;
    mTile.map_tile_width = tilemap->width;
    mTile.map_tile_height = tilemap->height;
    #endif
}

Tile Map::GetTileInfo()
{

    Tile temp;
#if 0
    temp.map_tile_height = mTile.map_tile_height;
    temp.map_tile_width = mTile.map_tile_width;
    temp.tile_width = mTile.tile_width;
    temp.tile_height = mTile.tile_height;
#endif
    return temp;
}

bool Map::IsTileObstacle(int x, int y)
{    
    tmx_layer *layer3 = tmx_find_layer_by_name(tilemap,"bottom");
    tmx_property *prop = tmx_get_property(layer3->properties, "coli");

    int gid = x / tilemap->tile_width + tilemap->width * y / tilemap->tile_height;
    std::cout<<gid<<std::endl;
    if(layer3->content.gids[gid] > 0)
    {
        if(prop->value.integer == 20)
        {
            return true;
        }
    }
    return false;
}

Vector2i Map::GetTileCoord(int x, int y)
{
    Vector2i cur_coord;
    cur_coord.x = x / tilemap->tile_width;
    cur_coord.y = y / tilemap->tile_height;
    return cur_coord;
}

Vector2i tilexy[50] = {{-1,-1}};
int cnt = 0;
int moverange = 2;
void Map::MoveRangeUpdate(Vector2i pos)
{
    if(tilexy[0].x == -1 && tilexy[0].y == -1)
    {
        Vector2i tile_coord = GetTileCoord(pos.x, pos.y);
        std::cout<<tile_coord.x<<"----"<<tile_coord.y<<std::endl;
        cnt = 0;
        for(int i = tile_coord.x -moverange; i <= tile_coord.x + moverange; i++)
        {
            for(int j = tile_coord.y - moverange ; j <= tile_coord.y + moverange; j++)
            {
                if(i >= 0 && j >= 0)
                {
                    if((abs(i-tile_coord.x)+abs(j-tile_coord.y)) <= moverange)
                    {
                        tilexy[cnt].x = i;
                        tilexy[cnt].y = j;
                        cnt++;
                    }
                }
            }
        }
        for(int i=0;i < cnt; i++)
        {
            std::cout<<"("<<tilexy[i].x<<","<<tilexy[i].y<<")"<<std::endl;
        }
    }
}

void Map::RenderMoveRange()
{
    int real_coord_x, real_coord_y;
    //std::cout<<cnt<<std::endl;
    for(int i = 0; i < cnt; i++)
    {
        tmx_layer *layer3 = tmx_find_layer_by_name(tilemap,"bottom");
        tmx_property *prop = tmx_get_property(layer3->properties, "coli");
        real_coord_x = tilexy[i].x * tilemap->tile_width;
        real_coord_y = tilexy[i].y * tilemap->tile_height;
        //std::cout<<"("<<real_coord_x<<","<<real_coord_y<<")"<<std::endl;
        //bool obstacle = IsTileObstacle(real_coord_x, real_coord_y);

        int gid = real_coord_x / tilemap->tile_width + tilemap->width * real_coord_y / tilemap->tile_height;
        //std::cout<<gid<<std::endl;
        if(layer3->content.gids[gid] > 0)
        {
            if(prop->value.integer == 20)
            {
                Window::renderTexture(immoveable, real_coord_x, real_coord_y, NULL);
            }
            else
            {
                Window::renderTexture(moveable, real_coord_x, real_coord_y, NULL);
            }

        }
        else
        {
            Window::renderTexture(moveable, real_coord_x, real_coord_y, NULL);
        }
        
        #if 0
        if(obstacle)
        {
            Window::renderTexture(immoveable, real_coord_x, real_coord_y, NULL);
        }
        else
        {
            Window::renderTexture(moveable, real_coord_x, real_coord_y, NULL);
        }
        #endif
    }
}

void Map::Load(std::string file)
{
    /* Set the callback globs in the main function */
    tmx_img_load_func = (void* (*)(const char*))Window::SDL_tex_loader;
    tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;
    #if 1
    tilemap = tmx_load("./resource/tiles/three_kingdom.tmx");
    if (!tilemap) {
        tmx_perror("Cannot load map");
    }
    #endif
    InitTileInfo();
    show_map_info(tilemap);
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

    /*
      0:起始x
      0:起始y
      960：0+960
      720：0+720
    */
    Recti dst = {0, 0, 960, 720};
    Recti clip = {0, 0, 960, 720};//地图固定大小

    Window::DrawTexture_ex(mapTexture, dst, &clip);

    RenderMoveRange();

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
    moveable = Window::LoadTexture("./resource/image/green.png");
    immoveable = Window::LoadTexture("./resource/image/red.png");
    SDL_Rect clip = {0, 0, 960, 720};
    //Window::renderTexture(mapTexture, 0, 0, &clip);
    
}
