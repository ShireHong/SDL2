#include <cmath>
#include "vectors.h"
#include "motionstate.h"

MotionState::MotionState()
{
	mState = IDLE;
}

MotionState::~MotionState()
{

}

void MotionState::UpdateState(Kinematic kinematic)
{
	//update state based on current motion
	if (fabs(kinematic.Vel.x) > 0.05 || fabs(kinematic.Vel.y) >= 0.05)
	{
		mState = RUNNING;
	}
	else
	{		
		mState = IDLE;
	}
}
int MotionState::State() const{
	return mState;
}
void MotionState::SetMotionState(int state){
	mState = state;
}
