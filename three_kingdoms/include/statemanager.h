#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "state.h"

///Handles picking a state to run and loading/saveing them
/**
*  Handles loading a state, running it and then based on 
*  on the value returned from the state loading the next state 
*  desired
*/
class StateManager {
public:
	/**
	*  Start the first state, mIntro
	*/
	static void InitIntro();
	/**
	*  Set a state active based on its name
	*  @param name The name of the state to set active
	*/
	static void RunState(std::string name);
    /**
    *  Get the active State
    *  @return A shared_ptr to the active state
    */
    
    static std::shared_ptr<State> GetActiveState();
	/**
	*  Load a state from file by its name
	*  @param name The state name to load for the state, state and file names are same
	*/
	static void LoadState(std::string name);
	#if 0
	/**
	*  Save a state to a data file corresponding to its name
	*  @param name The state name to save, state and file names are the same
	*/
	static void SaveState(std::string name);
	/**
	*  Change the active scene to the scene passed, exiting the current active scene
	*  @param scene The scene to change to
	*/
	static void ChangeScene(std::string scene);
	#endif
private:
	static std::shared_ptr<State> mActiveState;
};

#endif