#include "Arrive.h"

Arrive::Arrive(const Body* npc_, const Body* target_,float maxAcceleration_, float maxSpeed_, float targetRadius_, float slowRadius_, float timeToTarget_)
{
	npc = npc_;
	target = target_;
	maxAcceleration = maxAcceleration_;
	maxSpeed = maxSpeed_;
	targetRadius = targetRadius_;
	slowRadius = slowRadius_;
	timeToTarget = timeToTarget_;
}

Arrive::~Arrive()
{
}

SteeringOutput* Arrive::getSteering()
{
	result = new SteeringOutput();
	float targetSpeed;
	Vec3 direction = target->getPos() - npc->getPos();
	float distance = VMath::mag(direction);
	if (distance < targetRadius) {
		return nullptr;
	}

	if (distance > slowRadius) {
		 targetSpeed = maxSpeed;
	}
	else {
		targetSpeed = maxSpeed * distance / slowRadius;
	}

	
	target->getVel() = VMath::normalize(target->getVel()) * targetSpeed;
	result->linear = target->getVel() - npc->getVel();
	result->linear /= timeToTarget;

	if (VMath::mag(result->linear) > maxAcceleration) {
		result->linear = VMath::normalize(result->linear) * maxAcceleration;
	}

	result->angular = 0.0f;

	return nullptr;
}
