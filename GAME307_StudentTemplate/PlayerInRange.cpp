#include "PlayerInRange.h"

using namespace MATH;

// avoid circular dependency
#include "Character.h"

bool PlayerInRange::testValue()
{
	float threshold = 2.0f;
	if (VMath::distance(owner->getPlayerPos(), owner->getPos()) < threshold)
	{
		return true;
	}
	return false;
}