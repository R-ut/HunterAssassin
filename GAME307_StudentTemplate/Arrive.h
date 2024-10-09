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
	float timeToTarget;


public:
    Arrive(const Body* npc_, const Body* target_, float maxAcceleration_, float maxSpeed_,float targetRadius_, float slowRadius_, float timeToTarget_ = 0.1f);
    virtual ~Arrive();
    SteeringOutput* getSteering();
};
#endif 