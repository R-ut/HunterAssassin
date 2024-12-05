#ifndef ACTION_H
#define ACTION_H

#include "DecisionTreeNode.h"

enum class ACTION_SET {
    DO_NOTHING, 
    SEEK,
    PURSUE, 
    EVADE, 
    FLEE, 
    ARRIVE
};
class Action :
    public DecisionTreeNode
{
private:
    ACTION_SET value;

public:
    Action() : value{ ACTION_SET::DO_NOTHING }
    {}

    Action(ACTION_SET value_) : value{ value_ }
    {}

    ACTION_SET getValue() const
    {
        return value;
    }

    DecisionTreeNode* makeDecision()
    {
        return this;
    }
};

#endif

