#ifndef BLENDEDSTEERING_H
#define BLENDEDSTEERING_H

#include "SteeringBehaviour.h"
#include "KinematicBody.h"
#include <vector>

struct BehaviorAndWeight {
    SteeringBehaviour* behavior;
    float weight;
};

class BlendedSteering : public SteeringBehaviour {
public:
    BlendedSteering(KinematicBody* character) : SteeringBehaviour(character) {}
    ~BlendedSteering();

    void addBehavior(SteeringBehaviour* behavior, float weight);
    SteeringOutput* getSteering() override;

private:
    std::vector<BehaviorAndWeight> behaviors;
};

#endif