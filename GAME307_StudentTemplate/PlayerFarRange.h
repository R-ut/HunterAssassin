#ifndef PLAYER_FAR_RANGE_H
#define PLAYER_FAR_RANGE_H

#include "Decision.h"

class PlayerFarRange : public Decision {
private:
    bool testValue() override;

public:
    PlayerFarRange() : Decision() {}
    PlayerFarRange(DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_, Character* owner_)
        : Decision(trueNode_, falseNode_, owner_) {}

    ~PlayerFarRange() {}
};

#endif

