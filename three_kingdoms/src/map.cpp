#include <vector>
#include <set>
#include <string>
#include <string.h>
#include "map.h"
#include "res_path.h"
#include "window.h"
#include "camera.h"
#include "cleanup.h"

#define OPPOSITE_DIR(x) ((x + 2) % 4)

//Tile Map::mTile;
tmx_map* Map::tilemap;
Vector2i Map::maploc;


DirPos ArrayDir[] = {
                        {1,0},{0,1},{-1,0},{0,-1},/*right down left up*/
                        {-1,-1},{-1,1},{1,-1},{1,1},{0,0}
                   };

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

    int gid = x  + tilemap->width * y ;
    //std::cout<<gid<<std::endl;
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

Vector2i Map::GetRealCoord(int x, int y)
{
    Vector2i real_coord;
    real_coord.x = x * tilemap->tile_width;
    real_coord.y = y * tilemap->tile_height;
    return real_coord;
}

void Map::SetCursorPos(int x, int y)
{
    maploc.x = x / tilemap->tile_width * tilemap->tile_width;
    maploc.y = y / tilemap->tile_height * tilemap->tile_height;
}

Vector2i Map::GetCursorPos()
{
    return maploc;
}

QuadTreeNode rangelist[50] ;
int cnt = 0;
int moverange = 3;
int flag = 0;
QuadTreeNode path[10] ;
int cnt2 = 0;

QuadTreeNode realpath[10] ;

void Map::RangeScan(Vector2i pos, Direction dir, int cost)
{
    pos.x += ArrayDir[dir].x;
    pos.y += ArrayDir[dir].y;
    flag = 0;
    if(pos.x < 0 || pos.y < 0 || pos.x > tilemap->width || pos.y > tilemap->height)
    {
        return;
    }
    if(cost > moverange)
    {
        return;
    }
    if(!IsTileObstacle(pos.x, pos.y))
    {
        for(int i = 0; i < cnt; i++)
        {
            if(rangelist[i].tilepos.x == pos.x && rangelist[i].tilepos.y == pos.y)
            {
                if(cost < rangelist[i].cost)
                {
                    rangelist[i].NodeDir = dir;
                    rangelist[i].cost = cost;
                    //cout<<rangelist[i].x<<"----"<<rangelist[i].y<<" "<<rangelist[i].dic_x<<" "<<rangelist[i].dic_y<<" "<<rangelist[i].lvl<<endl;
                }
                else
                {
                    
                }
                flag = 1;
                break;
            }
        }
        if(!flag)
        {
            rangelist[cnt].tilepos= pos;
            rangelist[cnt].NodeDir = dir;
            rangelist[cnt].cost = cost;
            //cout<<rangelist[cnt].x<<"----"<<rangelist[cnt].y<<" "<<rangelist[cnt].dic_x<<" "<<rangelist[cnt].dic_y<<" "<<level<<endl;
            cnt++;
        }
        
    }
    else
    {
        return;
    }
    RangeScan(pos, D_LEFT, cost + 1);
	RangeScan(pos, D_UP, cost + 1);
	RangeScan(pos, D_DOWN, cost + 1);
	RangeScan(pos, D_RIGHT, cost + 1);

}

void Map::MoveRangeUpdate(Vector2i pos)
{
    memset(rangelist, 0, 50 * sizeof(QuadTreeNode));
    cnt = 0;
    Vector2i tile_coord = GetTileCoord(pos.x, pos.y);
    std::cout<<"coord "<<tile_coord.x<<"  "<<tile_coord.y<<std::endl;
    RangeScan(tile_coord, D_CENTER, 0);
    std::cout<<"cnt "<<cnt<<std::endl;
}

void Map::ClearMoveRange()
{
    memset(rangelist, 0, 50 * sizeof(QuadTreeNode));
    cnt = 0;
}

void Map::ClearMovePath()
{
    memset(path, 0, 10 * sizeof(QuadTreeNode));
    cnt2 = 0;
}


void Map::RenderMoveRange()
{
    Vector2i real_coord;
    //std::cout<<cnt<<std::endl;
    for(int i = 0; i < cnt; i++)
    {
        real_coord = GetRealCoord(rangelist[i].tilepos.x, rangelist[i].tilepos.y);
        Window::renderTexture(moveable, real_coord.x, real_coord.y, NULL);
    }
    for(int i = 0; i < cnt2; i++)
    {
        real_coord = GetRealCoord(path[i].tilepos.x, path[i].tilepos.y);
        // std::cout<<real_coord_x<<"----"<<real_coord_y<<std::endl;
        Window::renderTexture(immoveable, real_coord.x, real_coord.y, NULL);
    }
}

static bool GetRangeNode(Vector2i pos, QuadTreeNode* node)
{
    bool ret = true;
    int i;
    for(i = 0; i < cnt; i++)
    {
        // std::cout<<rangelist[i].tilepos.x<<"----"<<rangelist[i].tilepos.y<<std::endl;
        if(pos.x == rangelist[i].tilepos.x && pos.y == rangelist[i].tilepos.y)
        {
            *node = rangelist[i];
            ret = true;
            break;
        }
    }
    if(i == cnt)
    {
        ret = false;
    }
    return ret;
}

bool Map::GetMovePath(Vector2i end_pos, Vector2i start_pos, QuadTreeNode** ppath, int* step)
{
    Vector2i end = GetTileCoord(end_pos.x, end_pos.y);
    Vector2i start = GetTileCoord(start_pos.x, start_pos.y);
    bool ret = true;
    // std::cout<<"end "<<end.x<<"----"<<end.y<<std::endl;
    // std::cout<<"start "<<start.x<<"----"<<start.y<<std::endl;
    QuadTreeNode node;
    node.tilepos = end;
    node.NodeDir = D_CENTER;
    memset(path, 0, 10 * sizeof(QuadTreeNode));
    cnt2 = 0;

    do
    {
        /*记录的是上一次的方向，因为最后的目的地不需要方向*/
        ret = GetRangeNode(Vector2i(node.tilepos.x - ArrayDir[node.NodeDir].x, node.tilepos.y - ArrayDir[node.NodeDir].y), 
                            &node);
        if(ret)
        {
            path[cnt2].NodeDir = node.NodeDir;
            path[cnt2].tilepos = node.tilepos;
            std::cout<<" dir "<<node.NodeDir<<std::endl;
            cnt2++;
        }
        else
        {
            return false;
        }
    }while(node.tilepos.x != start.x || node.tilepos.y != start.y);

    for(int i = 0; i < cnt2; i++)
    {
        realpath[i].tilepos = GetRealCoord(path[cnt2-i-1].tilepos.x, path[cnt2-i-1].tilepos.y);
        realpath[i].NodeDir = path[cnt2-i-1].NodeDir;
        std::cout<<realpath[i].tilepos.x<<"----"<<realpath[i].tilepos.y<<"-----"<<realpath[i].NodeDir<<std::endl;
    }
    *ppath  = realpath;
    *step = cnt2;
    return true;
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
