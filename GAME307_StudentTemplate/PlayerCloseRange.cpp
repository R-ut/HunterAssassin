#include "PlayerCloseRange.h"
#include "Character.h"

bool PlayerCloseRange::testValue() {
    float threshold = 2.0f;  // if player is too close to character
    return VMath::distance(owner->getPlayerPos(), owner->getPos()) < threshold;
}

