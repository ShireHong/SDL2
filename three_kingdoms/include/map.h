#ifndef MAP_H
#define MAP_H

#include <memory>
#include <SDL_image.h>
#include "camera.h"

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
	//void Draw(std::string file);
	void RebuildMap();
	void Render();
	void Load(std::string file);
	void Draw(std::weak_ptr<Camera> cam);
	Recti Box() const;

	//void RebuildMap();
protected:
	SDL_Texture* mapTexture;
	tmx_map *tilemap;
	Recti mBox;
};

#endif