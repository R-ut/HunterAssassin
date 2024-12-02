#ifndef DECISION_H
#define DECISION_H

#include "DecisionTreeNode.h"
class Decision :
    public DecisionTreeNode
{
private:
    virtual bool testValue() = 0;
    virtual DecisionTreeNode* getBranch();

protected:
    DecisionTreeNode* trueNode;
    DecisionTreeNode* falseNode;
    class Character* owner;

public:
    Decision() : trueNode{ nullptr }, falseNode{ nullptr }, owner{ nullptr }
    {}

    Decision(DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_, Character* owner_) :
        trueNode{ trueNode_ },
        falseNode{ falseNode_ },
        owner{ owner_ }
    {}

    virtual DecisionTreeNode* makeDecision();
};

#endif
