#include "Seek.h"

Seek::Seek(const Body* npc_, Vec3 target_)
{
	npc = npc_;
	target = target_;
}

Seek::~Seek()
{
}

SteeringOutput* Seek::getSteering()
{
	//Get direction to target
	result->linear = target - npc->getPos();

	// accellerate in that direction
	result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
	result->angular = 0.0f;

	return result;
}
