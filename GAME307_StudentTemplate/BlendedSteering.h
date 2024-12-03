#ifndef BLENDEDSTEERING_H
#define BLENDEDSTEERING_H

#include "SteeringBehaviour.h"
#include "KinematicBody.h"
#include <vector>

struct BehaviorAndWeight {
    std::shared_ptr<SteeringBehaviour> behavior;
    float weight;
};

class BlendedSteering : public SteeringBehaviour {
public:
    BlendedSteering(KinematicBody* character) : SteeringBehaviour(character) {}
    ~BlendedSteering() = default;

    void addBehavior(std::shared_ptr<SteeringBehaviour> behavior, float weight);
    SteeringOutput* getSteering() override;

private:
    std::vector<BehaviorAndWeight> behaviors;
};

#endif