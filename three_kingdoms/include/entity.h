#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include "base.h"
#include "physics.h"
#include "map.h"
#include <SDL_image.h>

///The Entity class, to provide functions that call to attached Lua scripts
/**
*  Base class for Entities, has functions that simply provide a call to
*  the function of the same name on the Entity's attached Lua script
*/
class Entity {
public:
	Entity();
	/**
	*  Construct the Entity from a entity data file
	*  @param script The object's data file
	*/
	Entity(std::string file);
	virtual ~Entity();
	/**
    * Initialize the object and give the script the shared_ptr to the 
    * entity it's managing
    * @param self The shared_ptr to this entity that the manager is also using, so 
    * references are counted properly
    */
	virtual void Init(std::shared_ptr<Entity> self = nullptr);
	//Free the object's memory
	virtual void Free();
	///Update the game object
	virtual void Update();
	/**
	*  Move the object
	*  @param deltaT The elapsed time
	*/
	virtual void Move(float deltaT);
	/**
	*  Draw the Entity, apply an adjustment for the camera if one is desired
	*  @param cam The camera to adjust for
	*/
	virtual void Draw(std::weak_ptr<Camera> camera);
    //I'd like the various mouse interaction calls to pass the mouse position
	///On mouse down events
	virtual void OnMouseDown();
	///On mouse up event
	virtual void OnMouseUp();
    /**
    *  On mouse click event, a mouse click event is described
    *  as a mouse-down followed by a mouse-up
    */
    virtual void OnClick();
	///On mouse enter
	virtual void OnMouseEnter();
	///On mouse exit
	virtual void OnMouseExit();
	/**
	*  Check if mouse entered the object's box
	*  Updates mMouseOver accordingly
	*  @param pos The mouse pos to check 
	*/
	virtual void CheckMouseOver(const Vector2f &pos);
	///Return T/F is the mouse is over the object
	bool GetMouseOver() const;
	/**
	*  Get a pointer to the object's Physics component, for use in allowing
	*  it to be gotten and used in a script
    *  TODO: This should be removed right?
	*  @return A pointer to the entity's Physics member
	*/
	Physics* GetPhysics();
    /**
    *  Get a weak_ptr to the Entity's physics lib
    *  TODO: Migrate to using this exclusively
    *  @return a weak_ptr to the Physics component
    */
    std::weak_ptr<Physics> GetPhysicsWeakPtr();
	/**
	*  Set a collision map for the physics member to check against
	*  @param map The collision map
	*/
	void SetCollisionMap(CollisionMap map);
	///Get the object's box
	Rectf Box() const;
	///Set the entity's tag
	void SetTag(std::string tag);
	///Get the entity's tag
	std::string Tag() const;
    //Get and set Name
    void SetName(std::string name);
    std::string Name() const;
    ///Set the Entity render flag
    void Render(bool b);
    ///Check if the Entity should be drawn
    bool Render() const;
    ///Set the Entity "is ui element" flag
    void IsUiElement(bool b);
    ///Check if the Entity is a ui element
    bool IsUiElement() const;

	virtual void Load(std::string file);



protected:
	SDL_Texture* objTexture;
	///Handle's the objects physics
	std::shared_ptr<Physics> mPhysics;
	///A general tag for the entity
	std::string mTag;
	///The entity's name
	std::string mName;
    //The entity's file name
    std::string mConfigFile;
	///For tracking mouse over and clicks
	bool mMouseOver, mClicked;
    ///For tracking if the entity should be rendered, and if it's a ui element
    bool mRender, mUiElement;
};

#endif