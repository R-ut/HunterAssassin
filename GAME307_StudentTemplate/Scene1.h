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
#include "CollisionAvoidance.h"
#include "Flocking.h"
#include "BlendedSteering.h"
#include "PlayerBody.h"
#include "GameManager.h"

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
	float tileSize = 1.0f; // Size of each tile
	//calculate the number of rows and columns for the tiles
	int cols = ceil((xAxis - 0.5 * tileSize) / tileSize);
	int rows = ceil((yAxis - 0.5 * tileSize) / tileSize);

	KinematicBody* myNPC;
	Character* Enemy1;
	Vec3 cameraOffset;

	Graph* graph;
	std::vector<Node*> sceneNodes;

	
	std::vector<std::vector<Tile*>> tiles;
	// Flocking-related members
	std::vector<KinematicBody*> flockingNeighbors; // List of flocking neighbors
	void createTiles();
public:

	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event &event);
	//Highlight the explored tiles as well as the path tiles
	void highlightExploredTiles(Node* startNode, Node* targetNode);
	//Function to add a wall to the graph
	void addWallToGraph(int tileX, int tileY);
	
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
	void renderMyNPC();
};

#endif
