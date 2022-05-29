#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <set>
#include <memory>
#include <SDL_image.h>
///Handles the tile map
/**
*  Takes care of a simple tile based map
*/
class Map {
public:
	Map();
	~Map();
	/**
	*  Draw the tiles on the screen, relative to the camera
	*  @param cam The camera so we can get the offsets/check if things are in camera
	*/
	void Draw(std::string file);
	void RebuildMap();
protected:
	SDL_Texture* mapTexture;
};

#endif