#ifndef SCENE1_H
#define SCENE1_H

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


using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	float xAxis = 25.0f;
	float yAxis = 15.0f;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
    Matrix4     inverseProjection;
	SDL_Texture* backgroundTexture;  // Background texture
	std::vector<Wall*> walls; // Vector to store wall objects
	float BlockSize = 45.0f;
	float tileSize = 1.0f;

	KinematicBody* myNPC;
	Character* Enemy1;




	// 
	Graph* graph;
	std::vector<Node*> sceneNodes;

	
	std::vector<std::vector<Tile*>> tiles;
	void createTiles();
public:

	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void WallCollision(PlayerBody* player);
    void HandleEvents(const SDL_Event &event);
	const std::vector<Wall*>& getWalls() const;
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
	void renderMyNPC();
};

#endif
