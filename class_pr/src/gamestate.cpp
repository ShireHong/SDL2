#include <string>
#include <memory>
#include "window.h"
#include "gamestate.h"

#include <iostream>

GameState::GameState()
{
}
GameState::~GameState()
{
}
std::string GameState::Run()
{
	Init();
	while (!mExit)
	{

		//Rendering
		Window::Clear();
		mMap->Draw("111");

		Window::Present();
	}
	return mExitCode;
}

void GameState::Init(){
	mMap 	   = std::shared_ptr<Map>(new Map());
}