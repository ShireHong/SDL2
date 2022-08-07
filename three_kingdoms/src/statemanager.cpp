#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include "state.h"
#include "gamestate.h"
#include "statemanager.h"

std::shared_ptr<State> StateManager::mActiveState;

void StateManager::InitIntro(){
	RunState("gIntro");
}
void StateManager::RunState(std::string name){
    std::string nextState = name;
    do {
        LoadState(nextState);
        nextState = mActiveState->Run();
    }
    while (nextState != "quit");
 
}

std::shared_ptr<State> StateManager::GetActiveState()
{
    return mActiveState;
}

void StateManager::LoadState(std::string name)
{
    //Differentiate between menu and game states
    //Todo: will this be necessary? in the future if states are more scripted
    //the only differences should be in the script/json? yes, this change will come soon
    if (name.at(0) == 'm'){
        //mActiveState = std::make_shared<MenuState>();
    }
    else if (name.at(0) == 'g'){
        mActiveState = std::make_shared<GameState>();
    }
    else if (name.at(0) == 'e'){
        //mActiveState = std::make_shared<EditorState>();
    }
    //Currently log invalid state names however I think everything but
    //Editor state should be rolled into a single generic State with no letter prefix to its name
    else {
        //Debug::Log("StateManager: Invalid statename - " + name);
        exit(1);
    }
    //Load the state
    mActiveState->Load("./resource/tiles/three_kingdom.png");
}
