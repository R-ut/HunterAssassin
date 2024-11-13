#ifndef SEEK_H
#define SEEK_H
#include <VMath.h>
using namespace MATH;
#include "SteeringBehaviour.h"
class Seek :
    public SteeringBehaviour
{
protected:
    Vec3 target;

public:
    Seek(const Body* npc_, Vec3 target_);
    virtual ~Seek();
    SteeringOutput* getSteering();
};
#endif 
