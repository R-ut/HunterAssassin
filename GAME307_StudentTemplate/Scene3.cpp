#include "Scene3.h"
#include "MMath.h"

using namespace MATH;

Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* game_) {
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;
    cameraOffset = Vec3(0.0f, 0.0f, 0.0f);

    // Initialize enemy pointers
    enemy1 = nullptr;
    enemy2 = nullptr;
    enemy3 = nullptr;
    enemy4 = nullptr;

  //  flockingBehavior = nullptr;
    flockingBehavior1 = nullptr;
    flockingBehavior2 = nullptr;
    flockingBehavior3 = nullptr;
    flockingBehavior4 = nullptr;
  
    blendedSteering1 = nullptr;
    blendedSteering2 = nullptr;
    blendedSteering3 = nullptr;
    blendedSteering4 = nullptr;

    isFlockingEnabled = true; // Flocking is enabled by default
}

Scene3::~Scene3() {}

void Scene3::DrawDebugVector(Vec3 start, Vec3 direction, SDL_Color color) {
    Vec3 end = start + direction;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer,
        static_cast<int>(start.x - cameraOffset.x),
        static_cast<int>(start.y - cameraOffset.y),
        static_cast<int>(end.x - cameraOffset.x),
        static_cast<int>(end.y - cameraOffset.y));
}
bool Scene3::OnCreate() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Match Scene1's projection matrix setup
    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 5.0f);
    projectionMatrix = ndc * ortho;

    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    // Set player image to PacMan
    SDL_Surface* image;
    SDL_Texture* texture;

    image = IMG_Load("SandGhoul.gif");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    game->getPlayer()->setImage(image);
    game->getPlayer()->setTexture(texture);

    // Create 4 enemies and place them in different corners
    enemy1 = new Character();
    if (!enemy1->OnCreate(this) || !enemy1->setTextureWith("Blinky.png") ||
        !enemy1->readDecisionTreeFromFile("DecisionMaking.xml")) {
        std::cerr << "Error creating Enemy1 in Scene3.\n";
        return false;
    }
    enemy1->getBody()->setPos(Vec3(2.0f, 2.0f, 0.0f)); // bottom-left corner

    enemy2 = new Character();
    if (!enemy2->OnCreate(this) || !enemy2->setTextureWith("Pinky.png") ||
        !enemy2->readDecisionTreeFromFile("nearPlayer.xml")) {
        std::cerr << "Error creating Enemy2 in Scene3.\n";
        return false;
    }
    enemy2->getBody()->setPos(Vec3(xAxis - 3.0f, 6.0f, 0.0f)); // bottom-right corner

    enemy3 = new Character();
    if (!enemy3->OnCreate(this) || !enemy3->setTextureWith("Inky.png") ||
        !enemy3->readDecisionTreeFromFile("nearPlayer.xml")) {
        std::cerr << "Error creating Enemy3 in Scene3.\n";
        return false;
    }
    enemy3->getBody()->setPos(Vec3(2.0f, yAxis - 2.0f, 0.0f)); // top-left corner

    enemy4 = new Character();
    if (!enemy4->OnCreate(this) || !enemy4->setTextureWith("Clyde.png") ||
        !enemy4->readDecisionTreeFromFile("DecisionMaking.xml")) {
        std::cerr << "Error creating Enemy4 in Scene3.\n";
        return false;
    }
    enemy4->getBody()->setPos(Vec3(xAxis - 2.0f, yAxis - 2.0f, 0.0f)); // top-right corner

    // Set up flocking and blended steering
    flockNeighbors.push_back(enemy1->getBody());
    flockNeighbors.push_back(enemy2->getBody());
    flockNeighbors.push_back(enemy3->getBody());
    flockNeighbors.push_back(enemy4->getBody());

    flockingBehavior1 = new Flocking(enemy1->getBody(), flockNeighbors, 4.0f, 4.0f, 4.0f);
    flockingBehavior2 = new Flocking(enemy2->getBody(), flockNeighbors, 4.0f, 4.0f, 4.0f);
    flockingBehavior3 = new Flocking(enemy3->getBody(), flockNeighbors, 4.0f, 4.0f, 4.0f);
    flockingBehavior4 = new Flocking(enemy4->getBody(), flockNeighbors, 4.0f, 4.0f, 4.0f);

    // Set up blended steering
    blendedSteering1 = new BlendedSteering(enemy1->getBody());
    blendedSteering1->addBehavior(std::shared_ptr<SteeringBehaviour>(flockingBehavior1), 4.0f);

    blendedSteering2 = new BlendedSteering(enemy2->getBody());
    blendedSteering2->addBehavior(std::shared_ptr<SteeringBehaviour>(flockingBehavior2), 1.0f);

    blendedSteering3 = new BlendedSteering(enemy3->getBody());
    blendedSteering3->addBehavior(std::shared_ptr<SteeringBehaviour>(flockingBehavior3), 4.0f);

    blendedSteering4 = new BlendedSteering(enemy4->getBody());
    blendedSteering4->addBehavior(std::shared_ptr<SteeringBehaviour>(flockingBehavior4), 1.0f);

    float wallWidth = 1.57f;  // Increased width for better visibility
    float wallHeight = 1.57f; // Increased height for better visibility

    // Bottom boundary
    for (float x = 0.0f; x <= 25.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 0.8f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Top boundary
    for (float x = 0.0f; x <= 20.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 15.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Top boundary Camera Follow
    for (float x = 0.0f; x <= 27.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 19.0f, 0.0f), wallWidth, wallHeight, renderer));
    }
    // Left boundary
    for (float y = 0.0f; y <= 24.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(0.0f, y, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Right boundary
    for (float y = 0.0f; y <= 12.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(25.5f, y, 0.0f), wallWidth, wallHeight, renderer));
    }

    // right boundary camera follow
    for (float y = 0.0f; y <= 24.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(36.0f, y, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (lower part of the maze)
    for (float x = 0.0f; x <= 20.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 4.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (lower part of the maze) camera follow left side
    for (float x = 25.5f; x <= 32.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 3.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (lower part of the maze) camera follow left side
    for (float x = 25.5f; x <= 31.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 9.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (lower part of the maze) camera follow right side
    for (float x = 30.5f; x <= 35.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 6.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Vertical divider in the lower-left corner
    for (float y = 4.0f; y <= 8.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(5.0f, y, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (middle part of the maze)
    for (float x = 15.0f; x <= 25.5f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 8.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Small vertical wall near the center
    for (float y = 7.5f; y <= 8.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(15.0f, y, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Horizontal divider (upper part of the maze)
    for (float x = 0.0f; x <= 18.0f; x += 0.5f) {
        walls.push_back(new Wall(Vec3(x, 12.0f, 0.0f), wallWidth, wallHeight, renderer));
    }

    // Small vertical wall in the upper-right corner
    for (float y = 12.0f; y <= 15.0f; y += 0.5f) {
        walls.push_back(new Wall(Vec3(21.0f, y, 0.0f), wallWidth, wallHeight, renderer));
    }



    return true;
}

void Scene3::OnDestroy() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    // Clean up enemy objects
    if (enemy1) {
        enemy1->OnDestroy();
        delete enemy1;
        enemy1 = nullptr;
    }

    if (enemy2) {
        enemy2->OnDestroy();
        delete enemy2;
        enemy2 = nullptr;
    }

    if (enemy3) {
        enemy3->OnDestroy();
        delete enemy3;
        enemy3 = nullptr;
    }

    if (enemy4) {
        enemy4->OnDestroy();
        delete enemy4;
        enemy4 = nullptr;
    }

    // Clean up walls
    for (Wall* wall : walls) {
        delete wall;
    }
    walls.clear();

    // Clean up flocking and blended steering for each enemy
    if (flockingBehavior1) {
        delete flockingBehavior1;
        flockingBehavior1 = nullptr;
    }
    if (flockingBehavior2) {
        delete flockingBehavior2;
        flockingBehavior2 = nullptr;
    }
    if (flockingBehavior3) {
        delete flockingBehavior3;
        flockingBehavior3 = nullptr;
    }
    if (flockingBehavior4) {
        delete flockingBehavior4;
        flockingBehavior4 = nullptr;
    }

    if (blendedSteering1) {
        delete blendedSteering1;
        blendedSteering1 = nullptr;
    }
    if (blendedSteering2) {
        delete blendedSteering2;
        blendedSteering2 = nullptr;
    }
    if (blendedSteering3) {
        delete blendedSteering3;
        blendedSteering3 = nullptr;
    }
    if (blendedSteering4) {
        delete blendedSteering4;
        blendedSteering4 = nullptr;
    }

}
void Scene3::Update(float deltaTime) {
    // Update camera offset to follow the player
    Vec3 playerPos = game->getPlayer()->getPos();
    cameraOffset.x = playerPos.x - (xAxis / 2.0f);  // Center the camera around the player
    cameraOffset.y = playerPos.y - (yAxis / 2.0f);

    // Clamp camera offset within bounds
    cameraOffset.x = std::max(0.0f, std::min(cameraOffset.x, xAxis - xAxis / 2.0f));
    cameraOffset.y = std::max(0.0f, std::min(cameraOffset.y, yAxis - yAxis / 2.0f));

    collisionAvoidance->HandlePlayerEnemyCollision(game->getPlayer(), enemy1, renderer, cameraOffset);
    collisionAvoidance->HandlePlayerEnemyCollision(game->getPlayer(), enemy2, renderer, cameraOffset);
    collisionAvoidance->HandlePlayerEnemyCollision(game->getPlayer(), enemy3, renderer, cameraOffset);
    collisionAvoidance->HandlePlayerEnemyCollision(game->getPlayer(), enemy4, renderer, cameraOffset);

    // Handle enemy-wall collisions with camera offset
    HandleEnemyWallCollision(enemy1, cameraOffset);
    HandleEnemyWallCollision_(enemy2);
    HandleEnemyWallCollision_(enemy3);
    HandleEnemyWallCollision(enemy4, cameraOffset);

    // Update enemies with flocking and blended steering if enabled
    if (isFlockingEnabled) {
        auto updateEnemySteering = [&](Character* enemy, BlendedSteering* blendedSteering) {
            if (enemy && blendedSteering) {
                SteeringOutput* steering = blendedSteering->getSteering();
                Vec3 velocity = enemy->getBody()->getVel() + steering->linear * deltaTime;
                enemy->getBody()->setVel(velocity);
                Vec3 position = enemy->getBody()->getPos() + velocity * deltaTime;
                enemy->getBody()->setPos(position);
                delete steering;
            }
            };

        updateEnemySteering(enemy1, blendedSteering1);
        updateEnemySteering(enemy2, blendedSteering2);
        updateEnemySteering(enemy3, blendedSteering3);
        updateEnemySteering(enemy4, blendedSteering4);
    }
    // Update decision - making and positions for each enemy
    if (enemy1) enemy1->Update(deltaTime);
    if (enemy2) enemy2->Update(deltaTime);
    if (enemy3) enemy3->Update(deltaTime);
    if (enemy4) enemy4->Update(deltaTime);


    // Update the player
    if (game->getPlayer()) {
        game->getPlayer()->Update(deltaTime);
    }
}

void Scene3::HandleEnemyWallCollision(Character* enemy, const Vec3& cameraOffset) {
    if (!enemy) return; // Ensure the enemy exists

    Vec3 enemyPos = enemy->getBody()->getPos();
    Vec3 enemyVel = enemy->getBody()->getVel();
    float enemyWidth = 1.57f;
    float enemyHeight = 1.57f;
    float SmoothMovementValue = 0.1f;

    for (Wall* wall : walls) {
        // Get wall boundaries relative to the camera offset
        Vec3 wallPos = wall->getPosition() - cameraOffset;
        float wallWidth = wall->getWidth();
        float wallHeight = wall->getHeight();

        // Calculate bounding boxes for enemy and wall
        float enemyLeft = enemyPos.x;
        float enemyRight = enemyPos.x + enemyWidth;
        float enemyTop = enemyPos.y;
        float enemyBottom = enemyPos.y + enemyHeight;

        float wallLeft = wallPos.x - (wallWidth / 2.0f);
        float wallRight = wallPos.x + (wallWidth / 2.0f);
        float wallTop = wallPos.y - (wallHeight / 2.0f);
        float wallBottom = wallPos.y + (wallHeight / 2.0f);

        // Check if the enemy's bounding box overlaps the wall's bounding box
        if (enemyRight > wallLeft && enemyLeft < wallRight &&
            enemyBottom > wallTop && enemyTop < wallBottom) {

            // Calculate overlap amounts
            float overlapLeft = enemyRight - wallLeft;
            float overlapRight = wallRight - enemyLeft;
            float overlapTop = enemyBottom - wallTop;
            float overlapBottom = wallBottom - enemyTop;

            // Resolve collision on the x-axis
            if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
                enemyPos.x -= std::min(overlapLeft, SmoothMovementValue);


            }
            else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
                enemyPos.x += std::min(overlapRight, SmoothMovementValue);


            }

            // Resolve collision on the y-axis
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                enemyPos.y -= std::min(overlapTop, SmoothMovementValue);


            }
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                enemyPos.y += std::min(overlapBottom, SmoothMovementValue);


            }
        }
    }

    // Set updated position and velocity back to the enemy
    enemy->getBody()->setPos(enemyPos);

}
void Scene3::HandleEnemyWallCollision_(Character* enemy) {
    if (!enemy) return; // Ensure the enemy exists

    Vec3 enemyPos = enemy->getBody()->getPos();
    Vec3 enemyVel = enemy->getBody()->getVel();
    float enemyWidth = 1.57f;
    float enemyHeight = 1.57f;
    float SmoothMovementValue = 0.1f;

    for (Wall* wall : walls) {
        // Get wall boundaries
        Vec3 wallPos = wall->getPosition();
        float wallWidth = wall->getWidth();
        float wallHeight = wall->getHeight();

        // Calculate bounding boxes for enemy and wall
        float enemyLeft = enemyPos.x;
        float enemyRight = enemyPos.x + enemyWidth;
        float enemyTop = enemyPos.y;
        float enemyBottom = enemyPos.y + enemyHeight;

        float wallLeft = wallPos.x - (wallWidth / 2.0f);
        float wallRight = wallPos.x + (wallWidth / 2.0f);
        float wallTop = wallPos.y - (wallHeight / 2.0f);
        float wallBottom = wallPos.y + (wallHeight / 2.0f);

        // Check if the enemy's bounding box overlaps the wall's bounding box
        if (enemyRight > wallLeft && enemyLeft < wallRight &&
            enemyBottom > wallTop && enemyTop < wallBottom) {

            // Calculate overlap amounts
            float overlapLeft = enemyRight - wallLeft;
            float overlapRight = wallRight - enemyLeft;
            float overlapTop = enemyBottom - wallTop;
            float overlapBottom = wallBottom - enemyTop;

            // Resolve collision on the x-axis
            if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) {
                enemyPos.x -= std::min(overlapLeft, SmoothMovementValue);
            }
            else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom) {
                enemyPos.x += std::min(overlapRight, SmoothMovementValue);
            }

            // Resolve collision on the y-axis
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                enemyPos.y -= std::min(overlapTop, SmoothMovementValue);
            }
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                enemyPos.y += std::min(overlapBottom, SmoothMovementValue);
            }
        }
    }

    // Set updated position and velocity back to the enemy
    enemy->getBody()->setPos(enemyPos);
}


void Scene3::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);

    // Render the background (optional)
    SDL_Rect bgRect;
    bgRect.x = static_cast<int>(-cameraOffset.x);
    bgRect.y = static_cast<int>(-cameraOffset.y);
    bgRect.w = static_cast<int>(xAxis);
    bgRect.h = static_cast<int>(yAxis);

  /*  if (enemy1) {
        enemy1->render(cameraOffset, 0.15f, projectionMatrix, false);
    }


    if (enemy2) {
        enemy2->render(cameraOffset, 0.15f, projectionMatrix, true);
    }


    if (enemy3) {
        enemy3->render(cameraOffset, 0.15f, projectionMatrix, true);
    }


    if (enemy4) {
        enemy4->render(cameraOffset, 0.15f, projectionMatrix, false);
    }*/

    


    

    // Render walls
    for (Wall* wall : walls) {
        wall->Render(projectionMatrix, cameraOffset);
    }

    // Render enemies and debug vectors
    for (auto enemy : { enemy1, enemy2, enemy3, enemy4 }) {
        if (enemy) {
            enemy->render(cameraOffset, 0.15f, projectionMatrix, false);

            // Draw debug vectors for flocking forces
            if (isFlockingEnabled) {
                Flocking* behavior = nullptr;

                if (enemy == enemy1) behavior = flockingBehavior1;
                else if (enemy == enemy2) behavior = flockingBehavior2;
                else if (enemy == enemy3) behavior = flockingBehavior3;
                else if (enemy == enemy4) behavior = flockingBehavior4;

                if (behavior) {
                    Vec3 separation = behavior->calculateSeparation();
                    Vec3 alignment = behavior->calculateAlignment();
                    Vec3 cohesion = behavior->calculateCohesion();

                    DrawDebugVector(enemy->getBody()->getPos(), separation, { 255, 0, 0, 255 }); // Red for separation
                    DrawDebugVector(enemy->getBody()->getPos(), alignment, { 0, 255, 0, 255 }); // Green for alignment
                    DrawDebugVector(enemy->getBody()->getPos(), cohesion, { 0, 0, 255, 255 });  // Blue for cohesion
                }
            }
        }
    }

    // Render the player
    game->RenderPlayer(5.0f);

    SDL_RenderPresent(renderer);
}
void Scene3::HandleEvents(const SDL_Event& event) {
    // Pass events to the player
    game->getPlayer()->HandleEvents(event);

    // Handle toggling flocking with the 'F' key
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
        isFlockingEnabled = !isFlockingEnabled;
        std::cout << (isFlockingEnabled ? "Flocking Enabled" : "Flocking Disabled") << std::endl;
    }
}



