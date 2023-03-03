#ifndef MAP_H
#define MAP_H

#include <memory>
#include <SDL_image.h>
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
    bool IsTileObstacle(int x, int y);
    static Vector2i GetTileCoord(int x, int y);
    static void MoveRangeUpdate(Vector2i pos);
    void RenderMoveRange();
protected:
    SDL_Texture* mapTexture;
    SDL_Texture* moveable;
    SDL_Texture* immoveable;
    static tmx_map *tilemap;
    Recti mBox;
};

#endif