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

        
        //std::cout<<"run"<<std::endl;
        //Logic
        //mCamera->Update();
        mManager->Update();
        
        //mManager->SetCollisionMaps(mMap.get());
        //float deltaT = delta.Restart() / 1000.f;
        //Rendering
        Window::Clear();
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
    //std::cout<<mCamera->Box().pos.x<<" "<<mCamera->Box().pos.y<<" "<<mCamera->Box().w<<" "<<mCamera->Box().h<<std::endl;
    //std::cout<<mCamera->Scene()<<std::endl;
    std::shared_ptr<Entity> cursor = std::make_shared<Entity>("./resource/tiles/cursor.png");
    cursor->SetName("cursor");
    Vector2i pos_1 = {40, 40};

    cursor->GetPhysics()->selectpos = pos_1;
    cursor->GetPhysics()->SetPosition(pos_1);
    Recti box_1 = {pos_1,60,60};
    cursor->GetPhysics()->SetBox(box_1);
    //e->GetPhysics()->SetBox();
    mManager->Register(cursor);


    std::shared_ptr<Entity> guanyu = std::make_shared<Entity>("./resource/image/guanyu_horse.png");
    guanyu->SetName("guanyu_horse");
    Vector2i pos2 = {48, 128};
    guanyu->GetPhysics()->selectpos = pos2;
    guanyu->GetPhysics()->SetPosition(pos2);
    Recti box_2 = {pos2,48,64};
    guanyu->GetPhysics()->SetBox(box_2);
    //e->GetPhysics()->SetBox();
    mManager->Register(guanyu);
}

void GameState::Init()
{
	mMap 	   = std::shared_ptr<Map>(new Map());
	mCamera    = std::shared_ptr<Camera>(new Camera());	
	mManager   = std::shared_ptr<EntityManager>(new EntityManager());

	mManager->Register(mCamera);
}