#pragma once
#include "PlayerBody.h"
#include "KinematicBody.h"
#include "Wall.h"
#include <vector>

class CollisionAvoidance {
public:
    float collisionDistance = 1.0f;  // Distance to detect collisions between characters
    float smoothMovementValue = 0.05f;// Just so the npc doesn't move too fast or teleport
    void HandlePlayerNPCollision(PlayerBody* player, KinematicBody* npc, SDL_Renderer* renderer);
    void HandlePlayerWallCollision(PlayerBody* player, const std::vector<Wall*>& walls);
    void HandleNPCWallCollision(KinematicBody* npc, const std::vector<Wall*>& walls);
    void ResolveWallCollision(Vec3& position, float width, float height, const Wall* wall);
};
