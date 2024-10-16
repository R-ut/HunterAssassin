#ifndef ARRIVE_H
#define ARRIVE_H
#include "SteeringBehaviour.h"
class Arrive :
    public SteeringBehaviour
{
protected:
    const Body* target;
	float maxAcceleration;
	float maxSpeed;
	float targetRadius;
	float slowRadius;
	float timeToTarget = 0.1f;


public:
    Arrive(const Body* npc_, const Body* target_);
    virtual ~Arrive();
    SteeringOutput* getSteering();
};
#endif 