#ifndef MAP_H
#define MAP_H

#include <memory>
#include <SDL2/SDL_image.h>
#include "camera.h"

///Handles the tile map
/**
*  Takes care of a simple tile based map
*/
struct Tile
{
    int tile_width;
    int tile_height;
    int map_tile_width;
    int map_tile_height;
};

enum Direction{
    D_RIGHT,
    D_DOWN,
    D_LEFT,
    D_UP,
    D_CENTER = 8,
};

struct DirPos{
    int x;
    int y;
};


struct QuadTreeNode
{
    Vector2i    tilepos;
    Direction   NodeDir;
    int         cost;
};

class Map {
public:
    Map();
    ~Map();
    /**
    *  Draw the tiles on the screen, relative to the camera
    *  @param cam The camera so we can get the offsets/check if things are in camera
    */
    //void Draw(std::string file);
    void RebuildMap();
    void Render();
    void Load(std::string file);
    void Draw(std::weak_ptr<Camera> cam);
    Recti Box() const;
    void InitTileInfo();
    Tile GetTileInfo();
    //void RebuildMap();	
    static bool IsTileObstacle(int x, int y);
    static Vector2i GetTileCoord(int x, int y);
    static Vector2i GetRealCoord(int x, int y);
    static void MoveRangeUpdate(Vector2i pos);
    
    static void SetCursorPos(int x, int y);
    static Vector2i GetCursorPos();

    void RenderMoveRange();
    static void ClearMoveRange();
    static void ClearMovePath();
    static bool GetMovePath(Vector2i end_pos, Vector2i start_pos, QuadTreeNode** path, int* step);
    static void RangeScan(Vector2i pos, Direction dir, int cost);
protected:
    SDL_Texture* mapTexture;
    SDL_Texture* moveable;
    SDL_Texture* immoveable;
    static tmx_map *tilemap;
    Recti mBox;
    static Vector2i maploc;
};

#endif