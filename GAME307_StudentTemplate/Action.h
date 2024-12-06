#ifndef ACTION_H
#define ACTION_H

#include "DecisionTreeNode.h"

// Enum with explicit values for each action
enum class ACTION_SET {
    DO_NOTHING = 0, // Represents idle behavior
    SEEK = 1,       // Move toward a target
    PURSUE = 2,     // Chase a moving target
    EVADE = 3,      // Avoid a moving target
    ARRIVE = 4,     // Move to a target and stop
    FLEE = 5        // Move away from a target
};

class Action : public DecisionTreeNode {
private:
    ACTION_SET value;

public:
    // Constructors
    Action() : value{ ACTION_SET::DO_NOTHING } {}
    Action(ACTION_SET value_) : value{ value_ } {}

    // Getter for the value
    ACTION_SET getValue() const {
        return value;
    }

    // Make a decision (return self as this is a terminal node)
    DecisionTreeNode* makeDecision() override {
        return this;
    }
};

#endif


