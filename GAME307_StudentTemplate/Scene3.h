#ifndef SCENE3_H
#define SCENE3_H

#include <MMath.h>
#include <VMath.h>
#include "Scene.h"
#include "Character.h"
#include "Body.h"
#include "KinematicBody.h"
#include "Seek.h"
#include "Flee.h"
#include "Arrive.h"
#include "Pursue.h"
#include "Evade.h"
#include "Wall.h"
#include "Graph.h"
#include "Node.h"
#include "Tile.h"
#include "CollisionAvoidance.h"
#include "Flocking.h"
#include "BlendedSteering.h"
#include "PlayerBody.h"
#include "GameManager.h"

using namespace MATH;
class Scene3 : public Scene {
private:
    SDL_Window* window;
    float xAxis = 25.0f;
    float yAxis = 15.0f;
    SDL_Renderer* renderer;
    Matrix4 projectionMatrix;
    Matrix4     inverseProjection;

    std::vector<Wall*> walls; // Vector to store wall object
    SDL_Texture* backgroundTexture;  // Background texture

    // Enemy characters
    Character* enemy1;
    Character* enemy2;
    Character* enemy3;
    Character* enemy4;

	// Camera offset
    Vec3 cameraOffset;
    CollisionAvoidance* collisionAvoidance;

    // Flocking and Blended Steering
    std::vector<KinematicBody*> flockNeighbors;
    Flocking* flockingBehavior1;
    Flocking* flockingBehavior2;
    Flocking* flockingBehavior3;
    Flocking* flockingBehavior4;

	BlendedSteering* blendedSteering1;
	BlendedSteering* blendedSteering2;
	BlendedSteering* blendedSteering3;
	BlendedSteering* blendedSteering4;
  

    // Debugging
    bool isFlockingEnabled; // Flag to toggle flocking

    // Utility for drawing debug vectors
    void DrawDebugVector(Vec3 start, Vec3 direction, SDL_Color color);

public:
    Scene3(SDL_Window* sdlWindow_, GameManager* game_);
    ~Scene3();

    bool OnCreate() override;
    void OnDestroy() override;
    void Update(float deltaTime) override;
    void HandleEnemyWallCollision(Character* enemy, const Vec3& cameraOffset);
    void HandleEnemyWallCollision_(Character* enemy);
    void Render() override;
    void HandleEvents(const SDL_Event& event) override;

    // Accessor functions
    const std::vector<Wall*>& getWalls() const;
    float getxAxis() { return xAxis; }
    float getyAxis() { return yAxis; }
    SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
    Matrix4 getInverseMatrix() { return inverseProjection; }
};

#endif



