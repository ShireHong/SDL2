#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>
#include "entity.h"
#include "map.h"
#include "camera.h"

///Typedef for a vector of Entity shared pointers
///TODO: Should i instead store objects in a map by their name/tag?
typedef std::vector<std::shared_ptr<Entity>> EntityList;

///Handles the Entities in the active state
/**
* A class to simplify handling of the Entities, and simple
* collision interactions between them
* Note: When we actually care about optimization, all the for loops calling
* move, update, collision maps, draw, etc should be replaced with something more
* intelligent/faster
*/
class EntityManager {
public:
	EntityManager();
	~EntityManager();
	///Draw the entities
	virtual void Draw();
	///Update the objects
	virtual void Update();
	/**
	*  Move the entities
	*  @param deltaT The time elapsed
	*/
	virtual void Move(float deltaT);
	/**
	*  Setup Entity collision maps
	*  @param map The map to get the collision maps from
	*/
	void SetCollisionMaps(Map *map);
	/**
	*  Register an Entity to be managed by the manager
	*  @param obj The Entity to register
	*/
	void Register(std::shared_ptr<Entity> obj);
	/**
	*  Register the scene camera with the manager so that 
	*  it can be used in checking which objects to deal with
	*  @param camera The scene camera
	*/
	void Register(std::shared_ptr<Camera> camera);
    /**
    *  Remove an Entity with the shared_ptr passed
    *  @param entity The shared_ptr of the Entity to remove
    */
    void Remove(std::shared_ptr<Entity> entity);
    /**
    *  Get an Entity by its name
    *  @param name The name to lookup
    *  @return A shared_ptr to the entity, nullptr if lookup failed
    */
    std::shared_ptr<Entity> GetEntity(const std::string &name);
	/**
	*  Testing the new method for mouse handling
	*/
	virtual void CheckMouseEvents();
	/**
	*  Handle mouse clicks, run through the active entities
	*  find which was clicked, and call its response function
	*  @param mouseEvent The mouse event that we're processing
	*/
	virtual void HandleMouseEvent(const SDL_MouseButtonEvent &mouseEvent);
	/**
	*  Handle Mouse motion, check if the mouse entered/exited an Entity
	*  @param mouseEvent The mouse event that we're processing
	*/
	virtual void HandleMouseEvent(const SDL_MouseMotionEvent &mouseEvent);
	
    //Testing shared pointers in Lua
    void PrintSharedPtrCount();

private:
	/**
	*  Get a collision map of entities within some distance of the target
	*  @param target The Entity to check for entities near
	*  @param distance The distance to search away from the target for entities
	*  @return A collision map of entities near the target
	*/
	CollisionMap GetEntityCollisionMap(const Rectf &target, int distance = 2 * 16);

protected:
	///The list of entities in the scene
	EntityList mEntities;
	std::shared_ptr<Camera> mCamera;
};

#endif