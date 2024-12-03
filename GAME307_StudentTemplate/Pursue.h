#ifndef PURSUE_H
#define PURSUE_H

#include "Seek.h"

class Pursue : public Seek {
private:
    float maxPrediction;  // Maximum prediction time for pursue behavior

public:
    Pursue(const Body* npc_, const Body* target_, float maxPrediction_);
    ~Pursue();

    // Override steering behavior to add prediction logic
    SteeringOutput* getSteering() override;
};

#endif  // PURSUE_H


