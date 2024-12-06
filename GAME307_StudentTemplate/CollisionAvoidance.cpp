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

void CollisionAvoidance::HandlePlayerWallCollision(PlayerBody* player, const std::vector<Wall*>& walls, const Vec3& cameraOffset) {
    Vec3 playerPos = player->getPos();
    float playerWidth = 1.0f;  // Player width
    float playerHeight = 1.0f; // Player height

    for (const Wall* wall : walls) {
        ResolveWallCollision(playerPos, playerWidth, playerHeight, wall, cameraOffset);

        player->setPos(playerPos);
    }
}

void CollisionAvoidance::HandleNPCWallCollision(KinematicBody* npc, const std::vector<Wall*>& walls, const Vec3& cameraOffset) {
    Vec3 npcPos = npc->getPos();
    Vec3 npcVel = npc->getVel();
    float npcWidth = 1.0f;  // NPC width
    float npcHeight = 1.0f; // NPC height

    for (const Wall* wall : walls) {
        ResolveWallCollision(npcPos, npcWidth, npcHeight, wall, cameraOffset);
    }

    npc->setPos(npcPos);
    npc->setVel(npcVel);
}

void CollisionAvoidance::ResolveWallCollision(Vec3& position, float width, float height, const Wall* wall, const Vec3& cameraOffset) {
    // Adjust wall position by subtracting the camera offset
    float wallLeft = wall->getPosition().x - (wall->getWidth() / 2.0f) - cameraOffset.x;
    float wallRight = wall->getPosition().x + (wall->getWidth() / 2.0f) - cameraOffset.x;
    float wallTop = wall->getPosition().y - (wall->getHeight() / 2.0f) - cameraOffset.y;
    float wallBottom = wall->getPosition().y + (wall->getHeight() / 2.0f) - cameraOffset.y;

    // Check for overlap on the x-axis
    if (position.x + (width / 2.0f) > wallLeft && position.x - (width / 2.0f) < wallRight &&
        position.y + (height / 2.0f) > wallTop && position.y - (height / 2.0f) < wallBottom) {

        // Calculate overlap amounts
        float overlapLeft = (position.x + (width / 2.0f)) - wallLeft;
        float overlapRight = wallRight - (position.x - (width / 2.0f));
        float overlapTop = (position.y + (height / 2.0f)) - wallTop;
        float overlapBottom = wallBottom - (position.y - (height / 2.0f));

        // Resolve collision on the axis with the smallest overlap
        if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
            position.x -= overlapLeft;
        }
        else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
            position.x += overlapRight;
        }
        else if (overlapTop < overlapBottom) {
            position.y -= overlapTop;
        }
        else {
            position.y += overlapBottom;
        }
    }
}



void CollisionAvoidance::HandlePlayerEnemyCollision(PlayerBody* player, Character* enemy, SDL_Renderer* renderer, const Vec3& cameraOffset) {
    if (!player || !enemy) {
        std::cerr << "Error: player or enemy is nullptr in HandlePlayerEnemyCollision" << std::endl;
        return;
    }

    // Adjust positions for the camera offset
    Vec3 adjustedPlayerPos = player->getPos() - cameraOffset;
    Vec3 adjustedEnemyPos = enemy->getPos() - cameraOffset;

    // Bounding box dimensions
    float playerWidth = 1.0f, playerHeight = 1.0f;
    float enemyWidth = 0.5f, enemyHeight = 0.5f;

    // Check for collision using axis-aligned bounding boxes (AABB)
    bool xOverlap = (adjustedPlayerPos.x < adjustedEnemyPos.x + enemyWidth) &&
        (adjustedPlayerPos.x + playerWidth > adjustedEnemyPos.x);
    bool yOverlap = (adjustedPlayerPos.y < adjustedEnemyPos.y + enemyHeight) &&
        (adjustedPlayerPos.y + playerHeight > adjustedEnemyPos.y);

    static SDL_Texture* originalTexture = enemy->getBody()->getTexture();
    static bool isCollisionTexture = false;

    //if (xOverlap && yOverlap) {

    //    if (!isCollisionTexture) {
    //        // Change enemy's texture to indicate collision
    //        SDL_Surface* collisionImage = IMG_Load("Pinky.png");
    //        if (!collisionImage) {
    //            std::cerr << "Failed to load collision image: " << IMG_GetError() << std::endl;
    //            return;
    //        }
    //        SDL_Texture* collisionTexture = SDL_CreateTextureFromSurface(renderer, collisionImage);
    //        SDL_FreeSurface(collisionImage);

    //        if (collisionTexture) {
    //            enemy->getBody()->setTexture(collisionTexture);
    //            isCollisionTexture = true;
    //        }
    //    }
    //}
    //else {
    //    // No collision, reset texture
    //    if (isCollisionTexture && originalTexture) {
    //        enemy->getBody()->setTexture(originalTexture);
    //        isCollisionTexture = false;
    //    }
    //}
}














