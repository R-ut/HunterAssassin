#ifndef PLAYER_IN_RANGE_H
#define PLAYER_IN_RANGE_H

#include "Decision.h"

#include <VMath.h>

class PlayerInRange : public Decision
{
private:
    bool testValue();

public:
    PlayerInRange() : Decision()
    {}

    PlayerInRange(
        DecisionTreeNode* trueNode_,
        DecisionTreeNode* falseNode_,
        Character* owner_
    ) :
        Decision(trueNode_, falseNode_, owner_)
    {}


};

#endif

