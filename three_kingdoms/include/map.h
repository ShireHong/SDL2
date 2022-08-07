#ifndef MAP_H
#define MAP_H

<<<<<<< HEAD
#include <memory>
#include <SDL_image.h>
#include "camera.h"

=======
#include <vector>
#include <string>
#include <set>
#include <memory>
#include <tmx.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
>>>>>>> 96f14378fc7d1e52705c499877fd97229aaff2f0
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
<<<<<<< HEAD
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
=======
	//void RebuildMap();
	void Render();
	void Load(std::string file);
protected:
	static tmx_map *tilemap;
>>>>>>> 96f14378fc7d1e52705c499877fd97229aaff2f0
};

#endif