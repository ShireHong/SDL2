#include "state.h"

State::State() 
	: mName(""), mExit(false), 
	  mExitCode("quit")//, mCondBool(false)
{
}
State::~State(){}

std::string State::Run(){
	return mExitCode;
}

void State::SetName(std::string name){
	mName = name;
}
std::string State::Name(){
	return mName;
}