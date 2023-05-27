#include <vector>
#include <memory>
#include <iostream>
#include "input.h"
#include "map.h"
#include "entity.h"
#include "entitymanager.h"

EntityManager::EntityManager() : mCamera(nullptr)
{

}

EntityManager::~EntityManager()
{
    for (std::shared_ptr<Entity> e : mEntities)
	{
        e->Free();
	}
}

void EntityManager::Draw()
{
    //Weak camera ptr to pass to the entities
    for (std::shared_ptr<Entity> e : mEntities)
    {
        if (e->Render())
        {
            //if (e->IsUiElement() || mCamera->InCamera(e->Box()))
            {
                e->Draw(mCamera);
            }
        }
    }
}

void EntityManager::Update()
{
    //Check for mouse events
    CheckMouseEvents();
    //Call object's update functions
    #if 1
    for (std::shared_ptr<Entity> e : mEntities)
    {
        //Check for Ui/Non-Ui elements
        if (e->IsUiElement() || mCamera->InCamera(e->Box()))
        {
            e->Update();
        }
    }

    #endif
}

void EntityManager::Move(float deltaT)
{
    for (std::shared_ptr<Entity> e : mEntities){
        //Check for Ui/Non-Ui elements
        if (e->IsUiElement() || mCamera->InCamera(e->Box()))
        {
            e->Move(deltaT);
        }
    }
}

void EntityManager::SetCollisionMaps(Map *map)
{
#if 0
    //TODO: Is there a better way this can be done?
    CollisionMap collMap, entityMap;
    for (std::shared_ptr<Entity> e : mEntities)
    {
        if (!e->IsUiElement() && mCamera->InCamera(e->Box()))
        {
            CollisionMap collMap = map->GetCollisionMap(e->Box());
            CollisionMap entityMap = this->GetEntityCollisionMap(e->Box());
            for (Recti r : entityMap)
            {
                collMap.push_back(r);
            }

            e->SetCollisionMap(collMap);
            collMap.clear();
            entityMap.clear();
        }
    }
#endif
}

void EntityManager::Register(std::shared_ptr<Entity> obj)
{
	mEntities.push_back(obj);
}

void EntityManager::Register(std::shared_ptr<Camera> camera)
{
	mCamera.reset();
	mCamera = camera;
}

void EntityManager::Remove(std::shared_ptr<Entity> entity)
{
    //Check if the entity is in the vector
    EntityList::const_iterator iter;
    for (iter = mEntities.begin(); iter < mEntities.end(); ++iter)
	{
        if ((*iter)->Name() == entity->Name())
		{
            mEntities.erase(iter);
            return;
        }
    }
    //Debug::Log("Remove Entity Error: Entity " + entity->Name() + " not found");
}

 std::shared_ptr<Entity> EntityManager::GetEntity(const std::string &name)
 {
    //For now we do a really bad/slow lookup of looping through all entities
    //This will change when the EntityList container type is changed
    for (std::shared_ptr<Entity> e : mEntities)
    {
        if (e->Name() == name)
        {
            return e;
        }
        
    }
    return nullptr;
}

CollisionMap EntityManager::GetEntityCollisionMap(const Rectf &target, int distance)
{
    //TODO: Is there a better way this can be done? Looping through the entities is kind of ok, but
    //definitely not ideal
    CollisionMap entityMap;
    for (std::shared_ptr<Entity> e : mEntities)
    {
        if (!e->IsUiElement() && mCamera->InCamera(e->Box()))
        {
            double dist = Math::Distance(target.pos, e->Box().pos);
            if (dist > 0 && dist <= distance)
            {
                entityMap.push_back(e->Box());
            }
        }
    }
	return entityMap;
}

void EntityManager::CheckMouseEvents()
{
    //Check for mouse motion
    if (Input::MouseMotion())
    {
        //std::cout<<"move"<<std::endl;
        HandleMouseEvent(Input::GetMotion());
    }

    //Check for clicks
    if (Input::MouseClick(MOUSE::LEFT))
    {
        std::cout<<"click"<<std::endl;
        HandleMouseEvent(Input::GetClick());
    }
}

void EntityManager::HandleMouseEvent(const SDL_MouseButtonEvent &mouseEvent)
{
    //Update the mouse over before checking for clicks
    SDL_MouseMotionEvent tempEvt;
    Vector2i pos;
    tempEvt.x = mouseEvent.x;
    tempEvt.y = mouseEvent.y;
    HandleMouseEvent(tempEvt);
    //Find the object that was clicked
    switch (mouseEvent.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            /*check the entities*/
            for (std::shared_ptr<Entity> e : mEntities)
            {
                if ((e->IsUiElement() || mCamera->InCamera(e->Box())) && e->GetMouseOver())
                {
                    e->OnMouseDown();
                }
            }
            /*no entities choiced*/
            Map::SetCursorPos(tempEvt.x, tempEvt.y);
            std::cout<<"x:"<<tempEvt.x<<" "<<"y:"<<tempEvt.y<<std::endl;
            break;
        case SDL_MOUSEBUTTONUP:
            /*check the entities*/
            for (std::shared_ptr<Entity> e : mEntities)
            {
                if ((e->IsUiElement() || mCamera->InCamera(e->Box())) && e->GetMouseOver())
                {
                    e->OnMouseUp();
                }
            }
            break;
        default:
            break;
    }
}

void EntityManager::HandleMouseEvent(const SDL_MouseMotionEvent &mouseEvent)
{
    //Vector2f mousePos = Math::ToSceneSpace(mCamera.get(), Vector2f(mouseEvent.x, mouseEvent.y));
    Vector2f mousePos(mouseEvent.x, mouseEvent.y);
    //std::cout<<mousePos.x<<std::endl;
    //std::cout<<mousePos.y<<std::endl;
    //Find the object that has the mouse over it	

    #if 1
    for (std::shared_ptr<Entity> e : mEntities)
    {
        //Check for Ui/Non-Ui elements
        if (e->IsUiElement())
        {
            if (Math::CheckCollision(mousePos, e->Box()) && !e->GetMouseOver())
            {
                e->OnMouseEnter();
            }            
            else if (!Math::CheckCollision(mousePos, e->Box()) && e->GetMouseOver())
            {
                e->OnMouseExit();
            }
            
        }
        //If it's an object in the scene we must bump the collision box into
        //window space to check against the window space mouse pos
        else if (mCamera->InCamera(e->Box()))
        {
            Rectf box = Math::FromSceneSpace(mCamera, e->Box());
            //std::cout<<e->Box().pos.x<<" "<<e->Box().pos.y<<" "<<e->Box().w<<" "<<e->Box().h<<std::endl;
            if (Math::CheckCollision(mousePos, box) && !e->GetMouseOver())
            {		
                std::cout<<"in"<<std::endl;
                e->OnMouseEnter();
            }
            else if (!Math::CheckCollision(mousePos, box) && e->GetMouseOver())
            {
                e->OnMouseExit();
            }
        }
    }
    #endif
}


void EntityManager::PrintSharedPtrCount()
{
    //run through list and print count and name
    std::cout << "Entity shared_ptr counts" << std::endl;
    for (std::shared_ptr<Entity> e : mEntities)
    {
        std::cout << "Entity: " << e->Name() << " count: " << e.use_count() << std::endl;
    }
    }