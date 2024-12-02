#ifndef FLOCKING_H
#define FLOCKING_H

#include "SteeringBehaviour.h"
#include <vector>

class Flocking : public SteeringBehaviour {
public:
    Flocking(KinematicBody* character, std::vector<KinematicBody*> neighbors, float separationWeight, float alignmentWeight, float cohesionWeight)
        : SteeringBehaviour(character), neighbors(neighbors), separationWeight(separationWeight), alignmentWeight(alignmentWeight), cohesionWeight(cohesionWeight) {}

    SteeringOutput* getSteering() override;

private:
    std::vector<KinematicBody*> neighbors;
    float separationWeight;
    float alignmentWeight;
    float cohesionWeight;

    Vec3 calculateSeparation();
    Vec3 calculateAlignment();
    Vec3 calculateCohesion();
};

#endif