#ifndef PHYSICS_H
#define PHYSICS_H

#include <cmath>
#include <vector>
#include "base.h"
#include "math.h"
#include "motionstate.h"
#include "map.h"
/**
*  Ground friction value, this is should be changed to be configurable
*  perhaps based on floor type would be nice
*/
const int GROUND_FRICTION = 800;

///Provides physics functions for an object
/**
*  Handles an objects physics, collision checking, friction
*  motion, etc.
*  Note: A lot of the physics done here should actually be done by the user?
*/
class Physics {
public:
    Physics();
    ~Physics();
    /**
    *  Update the state of the object
    *  @param deltaT The elapsed time
    */
    void Move(float deltaT);
    ///Getters
    Vector2i Position() const;
    Vector2f Velocity() const;
    Vector2f Acceleration() const;
    Recti Box() const;
    int State() const;
    ///Setters
    void SetPosition(Vector2f pos);
    void SetVelocity(Vector2f vel);
    void SetAcceleration(Vector2f accel);
    ///Set the horizontal move direction @see Math enum
    void SetHorizDir(int moveDir);
    ///Set the vertical move direction @see Math enum
    void SetVertDir(int moveDir);
    /**
    *  Set the direction of motion
    *  @param v The vector direction to move in, will be normalized
    */
    void SetDirection(Vector2f v);
    void SetPhysConstants(PhysicalConstants physConstants);
    void SetBox(Recti box);
    void SetMap(CollisionMap map);
    //void Load(Json::Value val);	

    Vector2i selectpos;
    Vector2i targetpos;
    int state;
    QuadTreeNode *Path;
    int step;
    int cnt;
private:
	/**
	*  Updates the velocity by running the various acceleration functions
	*  @param deltaT The elapsed time
	*/
	void UpdateVelocity(float deltaT);
	/**
	* Apply the acceleration vector to the velocity vector
	*/
	void ApplyAcceleration();
	/**
	*  Apply friction to the object against the direction of motion
	*  based on its status, ground or air friction will be applied
	*/
	void ApplyFriction();
	/**
	*  Check for collision of the entity box with the local collision map
	*  @param box the entity's box, note that this may be a temp box for testing collision
	*  @return bool True if collision, False otherwise
	*/
	bool CheckCollision(Rectf box);

public:
	///Move enum, at the moment just has one for Stop as Math enum is used for dir @see Math
	enum MOVE { 
                STOP = 0,
                WALK,
                ATK,
              };

private:
	Kinematic mKinematic;
	MotionState mMotionState;
	PhysicalConstants mPhysConstants;
	Recti mBox;
	CollisionMap mCollisionMap;
	int mHorizDir, mVertDir;
    Vector2f mDir;
};

#endif