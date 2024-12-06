#include "PlayerFarRange.h"
#include "VMath.h"

#include "Character.h"
bool PlayerFarRange::testValue() {
    float threshold = 10.0f;  // if player is too far to character
    return VMath::distance(owner->getPlayerPos(), owner->getPos()) > threshold;
}
