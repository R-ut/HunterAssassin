#ifndef PLAYER_CLOSE_RANGE_H
#define PLAYER_CLOSE_RANGE_H

#include "Decision.h"
#include <VMath.h>
using namespace MATH;

class PlayerCloseRange : public Decision {
private:
    bool testValue() override;  

public:
    PlayerCloseRange() : Decision() {}
    PlayerCloseRange(DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_, Character* owner_)
        : Decision(trueNode_, falseNode_, owner_) {}

    ~PlayerCloseRange() {}
};

#endif


