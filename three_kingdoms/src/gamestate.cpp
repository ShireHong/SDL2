#include <string>
#include <memory>
#include <iostream>
#include "window.h"
#include "gamestate.h"
#include "input.h"
#include "timer.h"
#include "map.h"
#include "entity.h"

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
		{			
			SetExit("quit");
		}
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
		{
			SetExit("mIntro");
		}

		
		//Logic
		//mCamera->Update();
		mManager->Update();
		
		//mManager->SetCollisionMaps(mMap.get());
		//float deltaT = delta.Restart() / 1000.f;
		//Rendering
		Window::Clear();
		std::cout<<"run"<<std::endl;
		//mMap->Render();
		mMap->Draw(mCamera);
		mManager->Draw();
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

	std::shared_ptr<Entity> e = std::make_shared<Entity>("./resource/tiles/cursor.png");
	e->SetName("cursor");
	Vector2f pos = {40, 40};
	e->GetPhysics()->SetPosition(pos);
	//e->GetPhysics()->SetBox();
	mManager->Register(e);

	
}

void GameState::Init()
{
	mMap 	   = std::shared_ptr<Map>(new Map());
	mCamera    = std::shared_ptr<Camera>(new Camera());	
	mManager   = std::shared_ptr<EntityManager>(new EntityManager());
}