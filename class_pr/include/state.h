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
	void SetName(std::string name);
	///Get the state's name
	std::string Name();
protected:
	std::string mName;
	//Should this be atomic?
	bool mExit;
	std::string mExitCode;
};


#endif