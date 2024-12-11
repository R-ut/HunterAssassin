#include "Scene4.h"
#include <MMath.h>

using namespace MATH;
Scene4::Scene4(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	cameraOffset = Vec3(0.0f, 0.0f, 0.0f);
	// create a NPC
	myNPC = NULL;
	Enemy1 = nullptr;
}

Scene4::~Scene4() {}

bool Scene4::OnCreate() {

	//create tiles
	createTiles();

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 5.0f);
	projectionMatrix = ndc * ortho;

	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Load background image
	SDL_Surface* bgSurface = IMG_Load("tileset x1.png"); // Image Path
	if (bgSurface == nullptr) {
		std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
		return false;
	}
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
	SDL_FreeSurface(bgSurface);
	if (backgroundTexture == nullptr) {
		std::cerr << "Failed to create background texture: " << SDL_GetError() << "\n";
		return false;
	}
	//Lakshay's and deep's code , Rut had commented that out for his work. Sorry.
	// Set player image to PacMan
	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("SandGhoul.gif");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	// Set up myNPC kinematic character
	Vec3 position = Vec3(5.0f, 3.0f, 0.0f);
	Vec3 velocity = Vec3(5.0f, 0.0f, 0.0f);
	Vec3 acceleration = Vec3(0.0f, 0.0f, 0.0f);
	float mass = 1.0f;
	float radius = 0.5f;
	myNPC = new KinematicBody(
		position,
		velocity,
		acceleration,
		mass,
		radius

	);

	image = IMG_Load("Clyde.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	//error checking
	if (image == nullptr)
	{
		std::cerr << "Can't open clyde image\n";
		return false;
	}
	if (texture == nullptr)
	{
		std::cerr << "Can't open clyde texture\n";
		return false;
	}
	else
	{
		myNPC->setTexture(texture);
		SDL_FreeSurface(image);
	}


	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	Enemy1 = new Character();
	if (!Enemy1->OnCreate(this) || !Enemy1->setTextureWith("ToxicHound.gif"))
	{
		return false;
	}

	// Create neighbors for flocking
	// flockingNeighbors = { Enemy1->getBody(), Enemy2->getBody() }; // Populate with NPCs
	// Set up the graph
	graph = new Graph();
	if (!graph->OnCreate(sceneNodes)) {
		std::cerr << "Graph creation failed. Nodes may not be properly initialized.\n";
		return false;
	}

	// Add weighted connections between nodes in the graph
	//Firstly loop through the rows and cols ie go from bottom left to top right
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int currentLabel = i * cols + j; // to manover over the rows I am doing i * 25 + col
			Node* currentNode = sceneNodes[currentLabel];

			// Connect to the top neighbor
			if (i > 0) {
				int bottomNeighborLabel = (i - 1) * cols + j;//move to row bellow
				Node* bottomNeighbor = sceneNodes[bottomNeighborLabel];
				graph->addWeightedConnection(currentNode, bottomNeighbor, 1.0f);
			}

			// Connect to the bottom neighbor
			if (i < rows - 1) {
				int topNeighborLabel = (i + 1) * cols + j;//move to row above
				Node* topNeighbor = sceneNodes[topNeighborLabel];
				graph->addWeightedConnection(currentNode, topNeighbor, 1.0f);
			}

			// Connect to the left neighbor
			if (j > 0) {
				int leftNeighborLabel = i * cols + (j - 1);//move to left col
				Node* leftNeighbor = sceneNodes[leftNeighborLabel];
				graph->addWeightedConnection(currentNode, leftNeighbor, 1.0f);
			}

			// Connect to the right neighbor
			if (j < cols - 1) {
				int rightNeighborLabel = i * cols + (j + 1);//move to right col
				Node* rightNeighbor = sceneNodes[rightNeighborLabel];
				graph->addWeightedConnection(currentNode, rightNeighbor, 1.0f);
			}
		}
	}
	return true;
}

void Scene4::createTiles() {


	tiles.resize(rows);

	for (int i = 0; i < rows; i++) {
		tiles[i].resize(cols);
	}

	sceneNodes.resize(cols * rows);

	Node* n;
	int label = 0;
	Tile* t;
	int i = 0;
	int j = 0;
	for (float y = 0.5f * tileSize; y < yAxis; y += tileSize) {
		//Do stuff as y increases
		for (float x = 0.5f * tileSize; x < xAxis; x += tileSize) {
			//Do stuff as x increases
			Vec3 tilePos = Vec3(x, y, 0.0f);
			n = new Node(label);
			sceneNodes[label] = n;
			t = new Tile(n, tilePos, tileSize, tileSize, this);
			tiles[i][j] = t;
			j++;
			label++;
		}
		j = 0;
		i++;

	}
}
void Scene4::OnDestroy()
{
	if (backgroundTexture) {
		SDL_DestroyTexture(backgroundTexture);
		backgroundTexture = nullptr;
	}
	if (graph) {
		graph->~Graph();
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			tiles[i][j]->~Tile();
		}
	}




}



void Scene4::Update(const float deltaTime) {

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Matrix4 ndc = MMath::viewportNDC(w, h);

	float left, right, top, bottom;
	left = game->getPlayer()->getPos().x - 0.25f * xAxis;
	right = game->getPlayer()->getPos().x + 0.25f * xAxis;
	top = game->getPlayer()->getPos().y + 0.25f * yAxis;
	bottom = game->getPlayer()->getPos().y - 0.25f * yAxis;

	Matrix4 ortho = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	//game->getPlayer()->Update(deltaTime);


	int npcX = static_cast<int>(myNPC->getPos().x / tileSize);
	int npcY = static_cast<int>(myNPC->getPos().y / tileSize);
	Node* startNode = tiles[npcY][npcX]->getNode();

	int playerX = static_cast<int>(game->getPlayer()->getPos().x / tileSize);
	int playerY = static_cast<int>(game->getPlayer()->getPos().y / tileSize);

	Node* targetNode = tiles[playerY][playerX]->getNode();
	std::vector<Node*> exploredNodes;

	std::vector<Node*> path = graph->findPath(startNode, targetNode, exploredNodes);
	for (Node* node : path) {
		int label = node->getLabel();
		int tileX = label % cols;
		int tileY = label / cols;

		tiles[tileY][tileX]->setPath(true);
		Vec3 nodePos = tiles[tileY][tileX]->getPos();
		// Use the player as the target
		SteeringOutput* steering;
		Seek* seeker = new Seek(myNPC, game->getPlayer());
		steering = seeker->getSteering();
		myNPC->Update(deltaTime, steering);
	}
	if (myNPC) {
		// Update KinematicBody (myNPC)
		Vec3 position = myNPC->getPos();
		Vec3 velocity = myNPC->getVel();

		// Apply boundary checks
		float radius = myNPC->getRadius();
		float sceneWidth = game->getSceneWidth();
		float sceneHeight = game->getSceneHeight();

		float minX = radius;              // Left boundary
		float maxX = sceneWidth - radius; // Right boundary
		float minY = radius;              // Bottom boundary
		float maxY = sceneHeight - radius; // Top boundary

		if (position.x < minX) {
			position.x = minX;
			velocity.x = -velocity.x; // Reverse velocity
		}
		else if (position.x > maxX) {
			position.x = maxX;
			velocity.x = -velocity.x; // Reverse velocity
		}

		if (position.y < minY) {
			position.y = minY;
			velocity.y = -velocity.y; // Reverse velocity
		}
		else if (position.y > maxY) {
			position.y = maxY;
			velocity.y = -velocity.y; // Reverse velocity
		}

		// Update myNPC's position and velocity
		myNPC->setPos(position);
		myNPC->setVel(velocity);

	}

	// Update player
	if (game->getPlayer()) {
		game->getPlayer()->Update(deltaTime);
	}
}


void Scene4::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render the background
	SDL_Rect bgRect;
	bgRect.x = static_cast<int>(-cameraOffset.x);
	bgRect.y = static_cast<int>(-cameraOffset.y);
	bgRect.w = static_cast<int>(xAxis);
	bgRect.h = static_cast<int>(yAxis);
	//SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);

	//Rut overrode this work
	 //Rendering tiles
	for (int i = 0; tiles.size() > i; i++) {
		for (int j = 0; tiles[i].size() > j; j++) {
			tiles[i][j]->Render();
		}
	}


	renderMyNPC();
	// render the player
	game->RenderPlayer(5.10f);

	SDL_RenderPresent(renderer);
}
void Scene4::renderMyNPC()
{
	SDL_Rect rect;
	Vec3 screenCoords;
	int w, h;

	// convert coords
	screenCoords = projectionMatrix * myNPC->getPos();
	float scale = 0.15f;

	SDL_QueryTexture(myNPC->getTexture(), nullptr, nullptr, &w, &h);

	rect.w = static_cast<int>(w * scale);
	rect.h = static_cast<int>(h * scale);
	rect.x = static_cast<int>(screenCoords.x - 0.5 * rect.w);
	rect.y = static_cast<int>(screenCoords.y - 0.5 * rect.h);

	float orientation = myNPC->getOrientation();
	float degrees = orientation * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, myNPC->getTexture(), nullptr, &rect, degrees, nullptr, SDL_FLIP_NONE);
}

void Scene4::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	//when mouse clicked gtet the cordinatees of it
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		int mouseX = event.button.x;
		int mouseY = event.button.y;

		// Convert mouse click to world coordinates
		Vec3 worldPos = MMath::inverse(projectionMatrix) * Vec3(mouseX, mouseY, 0.0f);

		// Determine the clicked tile based on world coordinates
		//gails idea for getting the current tile
		int tileX = static_cast<int>(worldPos.x / tileSize);
		int tileY = static_cast<int>(worldPos.y / tileSize);

		int npcX = static_cast<int>(myNPC->getPos().x / tileSize);
		int npcY = static_cast<int>(myNPC->getPos().y / tileSize);
		//Checking if the player is in boundry
		if (tileX >= 0 && tileX < cols && tileY >= 0 && tileY < rows) {
			//Get the node using the tile row and column
			Node* selectedNode = tiles[tileY][tileX]->getNode();

			// Handle left and right mouse button clicks
			if (event.button.button == SDL_BUTTON_LEFT) {
				// Handle left-click for pathfinding
				Node* startNode = tiles[npcY][npcX]->getNode();
				//Get the node using the tile row and column
				highlightExploredTiles(startNode, selectedNode);
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				// Handle right-click to create a wall
				addWallToGraph(tileX, tileY);
				std::cout << "Wall added at: " << tileX << ", " << tileY << std::endl;
			}
		}
	}
}

void Scene4::highlightExploredTiles(Node* startNode, Node* targetNode) {
	// Clear previous highlights
	//Manover over rows and columns of tiles
	for (auto& row : tiles) {
		for (auto& tile : row) {
			tile->setExplored(false);
			tile->setPath(false);
		}
	}

	std::vector<Node*> exploredNodes;
	std::vector<Node*> path = graph->findPath(startNode, targetNode, exploredNodes);
	//create a path for explored nodes
	// Highlight explored tiles in orange
	for (Node* node : exploredNodes) {
		int label = node->getLabel();
		int tileX = label % cols;//get the col u are in
		int tileY = label / cols;//get the row u are in

		tiles[tileY][tileX]->setExplored(true);  // Mark as explored for orange
	}

	// Highlight final path tiles in red
	for (Node* node : path) {
		int label = node->getLabel();
		int tileX = label % cols;
		int tileY = label / cols;

		tiles[tileY][tileX]->setPath(true);  // Mark as path for red
	}
}
void Scene4::addWallToGraph(int tileX, int tileY) {
	Node* wallNode = tiles[tileY][tileX]->getNode();

	// Remove connections to this node in the graph, making it a "wall"
	for (Node* neighbor : graph->neighbours(wallNode)) {
		graph->addWeightedConnection(wallNode, neighbor, 0.0f);  // No connection
		graph->addWeightedConnection(neighbor, wallNode, 0.0f);  // No connection back
	}

	// Mark the tile as a wall for rendering
	tiles[tileY][tileX]->setWall(true);
}
