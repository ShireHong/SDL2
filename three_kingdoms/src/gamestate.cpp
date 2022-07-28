#include <string>
#include <memory>
#include "window.h"
#include "gamestate.h"
#include "input.h"
#include "timer.h"
#include "map.h"
#include <iostream>

GameState::GameState()
{

}

GameState::~GameState()
{

}

std::string GameState::Run()
{
	Input::Clear();
	//Cleanup any previous exit settings
	UnsetExit();

    //Call the script's Init
    State::Init();
    Timer delta;
	delta.Start();
	while (!mExit)
	{
		//EVENT POLLING
		Input::PollEvent();
		if (Input::Quit())
			SetExit("quit");
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
			SetExit("mIntro");

		//logic
		//float deltaT = delta.Restart() / 1000.f;
		//Rendering
		Window::Clear();
		//std::cout<<"run"<<std::endl;
		//mMap->Render();
		mMap->Draw(mCamera);
		Window::Present();

		Window::ShowAvgFps(false);
	}
	return mExitCode;
}

void GameState::Load(std::string file)
{
	Init();
	mMap->Load(file);
	mCamera->SetSceneBox(Rectf(0, 0, mMap->Box().w, mMap->Box().h));
	
}

void GameState::Init(){
	mMap 	   = std::shared_ptr<Map>(new Map());
	mCamera    = std::shared_ptr<Camera>(new Camera());
}