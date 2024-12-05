#ifndef FLOCKING_H
#define FLOCKING_H

#include "SteeringBehaviour.h"
#include <vector>

class Flocking : public SteeringBehaviour {
public:
    Flocking(KinematicBody* character, std::vector<KinematicBody*> neighbors,
        float separationWeight, float alignmentWeight, float cohesionWeight,
        float separationThreshold = 5.0f, float maxSeparation = 5.0f,
        float maxAlignment = 3.0f, float maxCohesion = 4.0f)
        : SteeringBehaviour(character), neighbors(neighbors),
        separationWeight(separationWeight), alignmentWeight(alignmentWeight),
        cohesionWeight(cohesionWeight), separationThreshold(separationThreshold),
        maxSeparation(maxSeparation), maxAlignment(maxAlignment), maxCohesion(maxCohesion) {}

    SteeringOutput* getSteering() override;

    // Make the following methods public
    Vec3 calculateSeparation();
    Vec3 calculateAlignment();
    Vec3 calculateCohesion();

private:
    std::vector<KinematicBody*> neighbors;
    float separationWeight;
    float alignmentWeight;
    float cohesionWeight;
    float separationThreshold; // Parameterized threshold
    float maxSeparation;
    float maxAlignment;
    float maxCohesion;

   
};

#endif