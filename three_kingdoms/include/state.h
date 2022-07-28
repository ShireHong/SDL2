#ifndef STATE_H
#define STATE_H

#include <string>
#include <memory>

///Base class for states
/**
*  The base class for our state machine, provides functions
*  for initializing state, running the state and memory clean-up
*/
class State {
public:
	State();
	virtual ~State();
	/**
	*  Run the state, this function becomes the main Input thread
	*  after starting up the physics and rendering threads
	*  @return The next state to run, returning quit exits program
	*/
	virtual std::string Run();
	/**
	*  Set the state's name
	*  @param name The name to set
	*/
	void SetExit(std::string val);
	///Set exit to false
	void UnsetExit();
	void SetName(std::string name);
	///Get the state's name
	std::string Name();

	virtual void Load(std::string file);
	
protected:
	/**
	*  The state's rendering thread, takes care of drawing all objects
	*  and providing framerate limiting condition variable notifications
	*  to all other threads
	*  NOT USED AT THE MOMENT
	*/
	//virtual void RenderThread() = 0;
	/**
	*  The state's physics thread, takes care of updating and moving
	*  all objects and managing physics between the objects
	*  NOT USED AT THE MOMENT
	*/
	//virtual void PhysicsThread() = 0;
	///Initialize state memory
	virtual void Init();
	///Free the memory used by the state
	virtual void Free();
	std::string mName;
	//Should this be atomic?
	bool mExit;
	std::string mExitCode;
};


#endif