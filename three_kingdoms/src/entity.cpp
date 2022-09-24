#include <string>
#include <fstream>
#include "math.h"
#include "entity.h"
#include "window.h"

Entity::Entity() : mPhysics(std::make_shared<Physics>()), mTag(""), mName(""), mConfigFile(""), 
    mMouseOver(false), mClicked(false), mRender(true), mUiElement(false)
{
}
Entity::Entity(std::string file) : mPhysics(std::make_shared<Physics>()), mTag(""), mName(""), mConfigFile(""),
    mMouseOver(false), mClicked(false), mRender(true), mUiElement(false)
{
    Load(file);
}
Entity::~Entity(){
}
void Entity::Init(std::shared_ptr<Entity> self)
{
	
}
void Entity::Free()
{

}
void Entity::Update()
{
    
}
void Entity::Move(float deltaT)
{
    //Move the object
    mPhysics->Move(deltaT);
}

void Entity::Draw(std::weak_ptr<Camera> camera)
{
    Window::renderTexture(objTexture, mPhysics->Position().x, mPhysics->Position().y, NULL);
}

void Entity::OnMouseDown()
{
    mClicked = true;

}
void Entity::OnMouseUp()
{
    //We have to do this here for now b/c ObjectButtons don't have 
    //an attached script, however I'd prefer to call OnClick after OnMouseUp
    if (mClicked)
        OnClick();
    mClicked = false;
	
}

void Entity::OnClick()
{

}

void Entity::OnMouseEnter()
{
    mMouseOver = true;
}

void Entity::OnMouseExit()
{
    mClicked = false;
    mMouseOver = false;

}

void Entity::CheckMouseOver(const Vector2f &pos)
{
	//Only trigger OnMouseEnter if the mouse is colliding and wasn't before
	if (Math::CheckCollision(pos, mPhysics->Box()) && !mMouseOver)
    {
		OnMouseEnter();
		mMouseOver = true;
	}
	//Only trigger mouse exit if the mouse was colliding, but isn't anymore
	else if (!Math::CheckCollision(pos, mPhysics->Box()) && mMouseOver)
    {
		OnMouseExit();
		mMouseOver = false;
	}
}

bool Entity::GetMouseOver() const 
{
	return mMouseOver;
}

Physics* Entity::GetPhysics()
{
	return mPhysics.get();
}

std::weak_ptr<Physics> Entity::GetPhysicsWeakPtr()
{
    std::weak_ptr<Physics> weak = mPhysics;
    return weak;
}

void Entity::SetCollisionMap(CollisionMap map)
{
	mPhysics->SetMap(map);
}

Rectf Entity::Box() const 
{
	return mPhysics->Box();
}
void Entity::SetTag(std::string tag)
{
	mTag = tag;
}
std::string Entity::Tag() const 
{
	return mTag;
}
void Entity::SetName(std::string name)
{
    mName = name;
}
std::string Entity::Name() const 
{
    return mName;
}
void Entity::Render(bool b)
{
    mRender = b;
}
bool Entity::Render() const 
{
    return mRender;
}
void Entity::IsUiElement(bool b)
{
    mUiElement = b;
}
bool Entity::IsUiElement() const 
{
    return mUiElement;
}

void Entity::Load(const std::string file)
{
    objTexture = Window::LoadTexture(file.c_str());
}

#if 0
Json::Value Entity::Save() const {
	//How to specify overrides to save?
    Json::Value val;
    if (mConfigFile != "")
        val["file"] = mConfigFile;
    else {
	    val["image"]   = mImage.File();
	    val["physics"] = mPhysics->Save();
	    val["tag"]	   = mTag;
	    val["script"]  = mScript->File();
	    val["name"]    = mName;
        val["render"]  = mRender;
        val["ui"]      = mUiElement;
    }
	return val;
}
void Entity::Save(const std::string &file) const {
    //Eventually will be merged in some manner with the other 
    //save function
    Json::Value val;
    val["image"]   = mImage.File();
	val["physics"] = mPhysics->Save();
	val["tag"]	   = mTag;
	val["script"]  = mScript->File();
	val["name"]    = mName;
    val["render"]  = mRender;
    val["ui"]      = mUiElement;
    JsonHandler handler(file);
    handler.Write(val);
}
void Entity::Load(Json::Value val){
    //Process overrides as well
	mTag  = val["tag"].asString();
	mName = val["name"].asString();
	mPhysics->Load(val["physics"]);
    mImage.Load(val["image"].asString());
    mRender = val["render"].asBool();
    mUiElement = val["ui"].asBool();
	mScript->OpenScript(val["script"].asString());
    //Store function and table references
    if (mScript->Open())
        StoreRefs();
}
void Entity::Load(const std::string &file, Json::Value overrides){
    mConfigFile = file;
    JsonHandler handler(mConfigFile);
    Json::Value data = handler.Read();
    data["overrides"] = overrides;
    Load(data);
}
void Entity::StoreRefs(){
    std::string table = mScript->TableName();
    mScript->Reference(table);
    mScript->Reference(table, "Init");
    mScript->Reference(table, "Free");
    mScript->Reference(table, "Update");
    mScript->Reference(table, "Move");
    mScript->Reference(table, "Draw");
    mScript->Reference(table, "OnMouseDown");
    mScript->Reference(table, "OnMouseUp");
    mScript->Reference(table, "OnClick");
    mScript->Reference(table, "OnMouseEnter");
    mScript->Reference(table, "OnMouseExit");
}
#endif
