#include "state.h"

State::State() 
	: mName(""), mExit(false), mExitCode("quit")//, mCondBool(false)
{

}

State::~State()
{

}

std::string State::Run()
{
	return mExitCode;
}

void State::SetExit(std::string val)
{
	mExit = true;
	mExitCode = val;
}

void State::UnsetExit(){
	mExit = false;
	mExitCode = "quit";
}

void State::Load(std::string file)
{
	
}

void State::SetName(std::string name)
{
	mName = name;
}

std::string State::Name(){
	return mName;
}

void State::Init()
{
    
}

void State::Free()
{
    
}