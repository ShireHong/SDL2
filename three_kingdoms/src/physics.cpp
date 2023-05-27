#include <cmath>
#include <iostream>
#include "math.h"
#include "motionstate.h"
#include "physics.h"

Physics::Physics(){
	mKinematic.Vel	 = Vector2f(0, 0);
	mKinematic.Accel = Vector2f(0, 0);
	mHorizDir = MOVE::STOP;
	mVertDir  = MOVE::STOP;
    mDir = Vector2f(0, 0);
    state  = MOVE::STOP;
    step = 0;
}
Physics::~Physics()
{

}

int velocity = 200;

void Physics::Move(float deltaT)
{
    Vector2i pos;
#if 1
    if(state == MOVE::WALK)
    {
        // for(int i = 0; i < cnt; i++)
        // {
        //     std::cout<<Path[i].tilepos.x<<"----"<<Path[i].tilepos.y<<"-----"<<Path[i].NodeDir<<std::endl;
        // }
        // state = MOVE::ATK;
        pos = Position();
        if(pos == targetpos)
        {
            std::cout<<"pos "<<pos.x<<"---"<<pos.y<<std::endl;
        }
        else
        {
            if(pos == Path[step].tilepos)
            {
                std::cout<<deltaT<<std::endl;
                step++;
                if(step == cnt)
                {
                    std::cout<<"step "<<step<<"---"<<cnt<<std::endl;
                    //state = MOVE::STOP;
                    //step = 0;
                }
            }
            else
            {
                // std::cout<<"Dir "<<Path[step].NodeDir<<std::endl;
                
                int offset = int(velocity *deltaT);
                switch(Path[step].NodeDir)
                {
                    case D_RIGHT:
                        pos.x += offset;
                        if(pos.x >= Path[step].tilepos.x)
                        {
                            pos.x = Path[step].tilepos.x;
                        }
                        break;
                    case D_DOWN:
                        pos.y += offset;
                        if(pos.y >= Path[step].tilepos.y)
                        {
                            pos.y = Path[step].tilepos.y;
                        }
                        break;
                    case D_LEFT:
                        pos.x -= offset;
                        if(pos.x <= Path[step].tilepos.x)
                        {
                            pos.x = Path[step].tilepos.x;
                        }
                        break;
                    case D_UP:
                        pos.y -= offset;
                        if(pos.y <= Path[step].tilepos.y)
                        {
                            pos.y = Path[step].tilepos.y;
                        }
                        break;
                    default:
                        std::cout<<"No direction!"<<std::endl;
                        return;
                }
                //std::cout<<"offset "<<offset<<std::endl;
                //std::cout<<"MOVE: "<<pos.x<<"----"<<pos.y<<std::endl;
                SetPosition(pos);
            }
        }
    }
#endif
}

void Physics::UpdateVelocity(float deltaT)
{
	ApplyAcceleration();
	ApplyFriction();
	//if applying the friction would cause the velocity to flip signs, set velocity to 0
	if (mHorizDir == MOVE::STOP && ((mKinematic.Vel.x + mKinematic.Accel.x * deltaT > 0.0 && mKinematic.Vel.x < 0.0)
		|| (mKinematic.Vel.x + mKinematic.Accel.x * deltaT < 0.0 && mKinematic.Vel.x > 0.0)))
	{
		mKinematic.Vel.x = 0.0;
	}
	else 
	{
		mKinematic.Vel.x += mKinematic.Accel.x * deltaT;
	}
	//same for y
	if (mVertDir == MOVE::STOP && ((mKinematic.Vel.y + mKinematic.Accel.y * deltaT > 0.0 && mKinematic.Vel.y < 0.0)
		|| (mKinematic.Vel.y + mKinematic.Accel.y * deltaT < 0.0 && mKinematic.Vel.y > 0.0)))
	{
		mKinematic.Vel.y = 0.0;
	}
	else
	{
		mKinematic.Vel.y += mKinematic.Accel.y * deltaT;
	}

	//limit velocity
	if ((mKinematic.Vel.x > mPhysConstants.hSpeed) || (mKinematic.Vel.x < -mPhysConstants.hSpeed))
	{
		mKinematic.Vel.x = mPhysConstants.hSpeed * (mKinematic.Vel.x / fabs(mKinematic.Vel.x));
	}
	//for y as well
	if ((mKinematic.Vel.y > mPhysConstants.hSpeed) || (mKinematic.Vel.y < -mPhysConstants.hSpeed))
	{
		mKinematic.Vel.y = mPhysConstants.hSpeed * (mKinematic.Vel.y / fabs(mKinematic.Vel.y));
	}
}

void Physics::ApplyAcceleration()
{
    mKinematic.Accel = mDir * mPhysConstants.hAccel;
    /*
	if (mHorizDir == Math::RIGHT)
		mKinematic.Accel.x = mPhysConstants.hAccel;
	else if (mHorizDir == Math::LEFT)
		mKinematic.Accel.x = -mPhysConstants.hAccel;
	else
		mKinematic.Accel.x = 0;
	if (mVertDir == Math::UP)
		mKinematic.Accel.y = -mPhysConstants.hAccel;
	else if (mVertDir == Math::DOWN)
		mKinematic.Accel.y = mPhysConstants.hAccel;
	else
		mKinematic.Accel.y = 0;
    */
}

void Physics::ApplyFriction()
{
	if (mHorizDir == MOVE::STOP && mKinematic.Vel.x != 0.0)
	{
		mKinematic.Accel.x += GROUND_FRICTION * (-mKinematic.Vel.x / fabs(mKinematic.Vel.x));
	}
	if (mVertDir == MOVE::STOP && mKinematic.Vel.y != 0.0)
	{
		mKinematic.Accel.y += GROUND_FRICTION * (-mKinematic.Vel.y / fabs(mKinematic.Vel.y));
	}
}

bool Physics::CheckCollision(Rectf box)
{
    //We wait to exit the loop until we've found the collision direction and
    //checked it against our movement direction to set v to 0 if moving in direction of 
    //the collision
    bool colliding = false;
    for (Recti i : mCollisionMap)
    {
        if (Math::CheckCollision(box, i))
        {
            colliding = true;
            //Check collision direction against movement directions
            int colDir = Math::RectNearRect(box, i, 15);
            if (mHorizDir == colDir)
            {
                mKinematic.Vel.x = 0;
                return colliding;
            }
            if (mVertDir == colDir)
            {
                mKinematic.Vel.y = 0;
                return colliding;
            }
        }
    }
    return colliding;
}

Vector2i Physics::Position() const
{
    return mBox.pos;
}

Vector2f Physics::Velocity() const
{
	return mKinematic.Vel;
}

Vector2f Physics::Acceleration() const
{
	return mKinematic.Accel;
}

Recti Physics::Box() const
{
	return mBox;
}

int Physics::State() const
{
	return mMotionState.State();
}

void Physics::SetPosition(Vector2f pos)
{
    mBox.pos.x = (pos.x / mBox.w) * mBox.w;
    mBox.pos.y = (pos.y / mBox.h) * mBox.h;
}

void Physics::SetVelocity(Vector2f vel)
{
	mKinematic.Vel = vel;
}

void Physics::SetAcceleration(Vector2f accel)
{
	mKinematic.Accel = accel;
}

void Physics::SetHorizDir(int moveDir)
{
	if (moveDir == Math::UP || moveDir == Math::DOWN)
	{
		//Debug::Log("Bad Horizontal Direction");
		std::cout<<"Bad Horizontal Direction"<<std::endl;
	}
	mHorizDir = moveDir;
    if (moveDir == Math::LEFT)
	{
        SetDirection(Vector2f(-1, mDir.y));
	}
    else if (moveDir == Math::RIGHT)
	{
        SetDirection(Vector2f(1, mDir.y));
	}
    else if (moveDir == MOVE::STOP)
	{
        SetDirection(Vector2f(0, mDir.y));
	}
}

void Physics::SetVertDir(int moveDir)
{
	if (moveDir == Math::LEFT || moveDir == Math::RIGHT)
	{
		//Debug::Log("Bad Vertical Direction");
		std::cout<<"Bad Vertical Direction"<<std::endl;
	}
	mVertDir = moveDir;
    if (moveDir == Math::UP)
	{
        SetDirection(Vector2f(mDir.x, -1));
	}
    else if (moveDir == Math::DOWN)
	{
        SetDirection(Vector2f(mDir.x, 1));
	}
    else if (moveDir == MOVE::STOP)
	{
        SetDirection(Vector2f(mDir.x, 0));
	}
}

void Physics::SetDirection(Vector2f v)
{
    mDir = Math::Normalize(v);
    //Backwards compatiability garbage
    if (mDir.x > 0)
	{
        mHorizDir = Math::RIGHT;
	}
    else if (mDir.x < 0)
	{
        mHorizDir = Math::LEFT;
	}
    else
	{
        mHorizDir = MOVE::STOP;
	}
    if (mDir.y > 0)
	{
        mVertDir = Math::DOWN;
	}
    else if (mDir.y < 0)
	{
        mVertDir = Math::UP;
	}
    else
	{
        mVertDir = MOVE::STOP;
	}
}

void Physics::SetPhysConstants(PhysicalConstants physConstants)
{
	mPhysConstants = physConstants;
}

void Physics::SetBox(Recti box)
{
	mBox = box;
}

void Physics::SetMap(CollisionMap map)
{
	mCollisionMap = map;
}
#if 0
void Physics::Load(Json::Value val)
{
	mBox.Load(val["box"]);
	mPhysConstants.hAccel = val["hAccel"].asInt();
	mPhysConstants.hSpeed = val["hSpeed"].asInt();

	//Setup motion values
	mKinematic.Vel	 = Vector2f(0, 0);
	mKinematic.Accel = Vector2f(0, 0);
	mHorizDir = MOVE::STOP;
	mVertDir  = MOVE::STOP;
}
#endif