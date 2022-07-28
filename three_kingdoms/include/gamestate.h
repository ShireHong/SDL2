#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <memory>
#include "state.h"
#include "map.h"

///A state for running a game level
/**
*  A simple game state that can run a level of the game
*/
class GameState : public State{
public:
	GameState();
	~GameState();
	/**
	*  Run the state, this function becomes the main Input thread
	*  after starting up the physics and rendering threads
	*  @return The next state to run, returning quit exits program
	*/
	std::string Run();

protected:
	/**
	*  The state's rendering thread, takes care of drawing all objects
	*  and providing framerate limiting condition variable notifications
	*  to all other threads
	*  NOT USED AT THE MOMENT
	*/
	//void RenderThread();
	/**
	*  The state's physics thread, takes care of updating and moving
	*  all objects and managing physics between the objects
	*  NOT USED AT THE MOMENT
	*/
	//void PhysicsThread();
	///Initialize state memory
	void Init();

	void Load(std::string file);
private:
	std::shared_ptr<Map> mMap;

};

#endif