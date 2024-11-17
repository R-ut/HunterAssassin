#include "CollisionAvoidance.h"

void CollisionAvoidance::HandlePlayerNPCollision(PlayerBody* player, KinematicBody* npc, SDL_Renderer* renderer) {
    static SDL_Texture* originalTexture = npc->getTexture(); // Store the original texture
    static bool isCollisionTexture = false; // Keep track of the current texture state

    // Calculate the distance between the player and NPC
    float pointDistance1 = VMath::mag(player->getPos() - npc->getPos());

    // If they are within the collision distance, handle the collision
    if (pointDistance1 < collisionDistance) {
        // Load the collision texture if not already set
        if (!isCollisionTexture) {
            SDL_Surface* collisionImage = IMG_Load("Inky.png");
            if (!collisionImage) {
                std::cerr << "Failed to load collision image: " << IMG_GetError() << std::endl;
                return;
            }
            SDL_Texture* collisionTexture = SDL_CreateTextureFromSurface(renderer, collisionImage);
            SDL_FreeSurface(collisionImage);  // Free the surface after creating the texture

            // Change NPC texture to the collision texture
            npc->setTexture(collisionTexture);
            isCollisionTexture = true; // Mark as collision texture
        }

        // Calculate new direction for NPC to move away
        Vec3 newDirection = VMath::normalize(npc->getPos() - player->getPos());

        // Move NPC to a new position to prevent overlapping
        Vec3 newPosition = npc->getPos() + newDirection * (collisionDistance - pointDistance1);
        npc->setPos(newPosition);

        // Set NPC velocity in the new direction
        npc->setVel(newDirection);
    }
    else {
        // Reset to the original texture if no collision
        if (isCollisionTexture) {
            npc->setTexture(originalTexture); // Revert to original texture
            isCollisionTexture = false; // Mark as original texture
        }
    }
}

void CollisionAvoidance::HandlePlayerWallCollision(PlayerBody* player, const std::vector<Wall*>& walls) {
    Vec3 playerPos = player->getPos();
    float playerWidth = 1.0f;  // Player width
    float playerHeight = 1.0f; // Player height

    for (const Wall* wall : walls) {
        ResolveWallCollision(playerPos, playerWidth, playerHeight, wall);
    }

    player->setPos(playerPos);
}

void CollisionAvoidance::HandleNPCWallCollision(KinematicBody* npc, const std::vector<Wall*>& walls) {
    Vec3 npcPos = npc->getPos();
    Vec3 npcVel = npc->getVel();
    float npcWidth = 1.0f;  // NPC width
    float npcHeight = 1.0f; // NPC height

    for (const Wall* wall : walls) {
        ResolveWallCollision(npcPos, npcWidth, npcHeight, wall);
    }

    npc->setPos(npcPos);
    npc->setVel(npcVel);
}

void CollisionAvoidance::ResolveWallCollision(Vec3& position, float width, float height, const Wall* wall) {
    float wallLeft = wall->getPosition().x;
    float wallRight = wallLeft + wall->getWidth();
    float wallTop = wall->getPosition().y;
    float wallBottom = wallTop + wall->getHeight();

    // Check x-axis collision
    if (position.x + width > wallLeft && position.x < wallRight &&
        position.y + height > wallTop && position.y < wallBottom) {

        float overlapLeft = (position.x + width) - wallLeft;
        float overlapRight = wallRight - position.x;

        if (overlapLeft < overlapRight) {
            position.x -= std::min(overlapLeft, smoothMovementValue);
        }
        else {
            position.x += std::min(overlapRight, smoothMovementValue);
        }

    }

    // Check y-axis collision
    if (position.x + width > wallLeft && position.x < wallRight &&
        position.y + height > wallTop && position.y < wallBottom) {

        float overlapTop = (position.y + height) - wallTop;
        float overlapBottom = wallBottom - position.y;

        if (overlapTop < overlapBottom) {
            position.y -= std::min(overlapTop, smoothMovementValue);
        }
        else {
            position.y += std::min(overlapBottom, smoothMovementValue);
        }
    }
}