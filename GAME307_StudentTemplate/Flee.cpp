#include "Flee.h"

Flee::Flee(const Body* npc_, const Body* target_)
{
	npc = npc_;
	target = target_;
}

Flee::~Flee()
{

}

SteeringOutput* Flee::getSteering()
{
	//Get direction to target
	result->linear = npc->getPos() - target->getPos();

	// accellerate in that direction
	result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
	result->angular = 0.0f;

	return result;
}
