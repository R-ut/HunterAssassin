#include "Scene1.h"

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	// create a NPC
	myNPC = NULL;
	Enemy1 = nullptr;
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {


	createTiles(); 
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
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
	
	// Initialize walls at desired positions (position, width, height, renderer)
	walls.push_back(new Wall(Vec3(0, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(50, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(100, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(150, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(200, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(250, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(300, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(350, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(400, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(450, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(500, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(550, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(600, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(650, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(700, 100, 0), BlockSize, BlockSize, renderer));
	//walls.push_back(new Wall(Vec3(750, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(800, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(850, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(900, 100, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(950, 100, 0), BlockSize, BlockSize, renderer));

	walls.push_back(new Wall(Vec3(0, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(50, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(100, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(150, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(200, 250, 0), BlockSize, BlockSize, renderer));
//	walls.push_back(new Wall(Vec3(250, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(300, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(350, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(400, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(450, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(500, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(550, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(600, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(650, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(700, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(750, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(800, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(850, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(900, 250, 0), BlockSize, BlockSize, renderer));
	walls.push_back(new Wall(Vec3(950, 250, 0), BlockSize, BlockSize, renderer));
	// Continue to add as many walls as needed

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
	if (!Enemy1->OnCreate(this) || !Enemy1->setTextureWith("ToxicHound.gif") )
	{
		return false;
	}

	// end of character set ups

	return true;
}

void Scene1::createTiles() {
	singleTile = new Tile(Vec3(0, 0, 0), 5, 3, this);
}
void Scene1::OnDestroy() 
{
	if (backgroundTexture) {
		SDL_DestroyTexture(backgroundTexture);
		backgroundTexture = nullptr;
	}
	if (Enemy1)
	{
		Enemy1->OnDestroy();
		delete Enemy1;
	}
	// Clean up walls
	for (Wall* wall : walls) {
		delete wall;
	}
	walls.clear();
}
//Creating the enum class to visually Compare between different kind of steering.
enum class BehaviorState {
	Pursuing,
	Evading,
	Arriving,
	Seeking,
	None
};
// Initializing current state which is set to NONE for now.
BehaviorState currentState = BehaviorState::None;

void Scene1::Update(const float deltaTime) {

	// Calculate and apply any steering for npc's
	Enemy1->Update(deltaTime);
	SteeringOutput* steering;
	float distance = VMath::mag(game->getPlayer()->getPos() - myNPC->getPos());
	BehaviorState newState = currentState;

	//Create a seek steering if the player is more than 2 units away
	//else create a flee steering
	if (distance < game->getPlayer()->getRadius() * 4) {

		// Calculate the player's velocity
		Vec3 playerVelocity = game->getPlayer()->getVel();
		float playerSpeed = VMath::mag(playerVelocity);

		if (playerSpeed > 0.1f) {
			newState = BehaviorState::Pursuing;
			Pursue* pursuer = new Pursue(myNPC, game->getPlayer(), 20.0f);// Max Prediction of 0.5
			steering = pursuer->getSteering();

		}
		else {
			newState = BehaviorState::Arriving;
			Arrive* Arriver;
			Arriver = new Arrive(myNPC, game->getPlayer());
			steering = Arriver->getSteering();

		}
	}
	else {
		if (distance > game->getPlayer()->getRadius() * 8) {
			newState = BehaviorState::Seeking;
			Seek* Seeker;
			Seeker = new Seek(myNPC, game->getPlayer());
			steering = Seeker->getSteering();
		}

		else {
		newState = BehaviorState::Evading;
		// Player is in the middle distance, evade them
		Evade* evader = new Evade(myNPC, game->getPlayer(), 20.0f);  // Max prediction of 1.0
		steering = evader->getSteering();

	}
	}
	// Setting the enum State with messages.
	if (newState != currentState) {
		switch (newState) {
		case BehaviorState::Pursuing:
			std::cout << "Pursuing" << std::endl;
			break;
		case BehaviorState::Evading:
			std::cout << "Evading" << std::endl;
			break;
		case BehaviorState::Arriving:
			std::cout << "Arriving" << std::endl;
			break;
		case BehaviorState::Seeking:
			std::cout << "Seeking" << std::endl;
			break;
		default:
			break;
		}

		// Update the current state to the new state as myNPC steers.
		currentState = newState;
	}
	
	if (!steering)
	{
		myNPC->setVel(Vec3(0.0f, 0.0f, 0.0f));
	}
	
	myNPC->Update(deltaTime, steering);

	// Calling the methods below
	float radius = myNPC->getRadius();
	Vec3 position = myNPC->getPos();
	Vec3 velocity = myNPC->getVel();

	float height, width;
	height = game->getSceneHeight();
	width = game->getSceneWidth();

	float minX = radius; // Left boundary
	float maxX = width - radius; // Right boundary
	float minY = radius; // Bottom boundary
	float maxY = height - radius; // Top boundary

	//Setting myNPC position and velocity to keep them within the boundaries
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

	// Set the new position and velocity to myNPC
	myNPC->setPos(position);
	myNPC->setVel(velocity);

	// Update player
	game->getPlayer()->Update(deltaTime);

	float collisionDistance = 1.0f;  // Collison Detection Distance so Characters don't overlap.
	float pointDistance1 = VMath::mag(game->getPlayer()->getPos() - myNPC->getPos());

	SDL_Surface* collisionImage = IMG_Load("Inky.png");
	SDL_Texture* collisionTexture = SDL_CreateTextureFromSurface(renderer, collisionImage);

	// Free the collision image surface after creating the texture
	SDL_FreeSurface(collisionImage);

	if (pointDistance1 < collisionDistance) {
		std::cout << "Collision Detected!!!" << std::endl;

		myNPC->setTexture(collisionTexture);// Changing myNPC texture when collided.

		// Calculating Direction myNPC needs to go after Collision
		Vec3 New_direction = VMath::normalize(myNPC->getPos() - game->getPlayer()->getPos());

		// Move myNPC to a new position to prevent overlaping.
		Vec3 New_Position = myNPC->getPos() + New_direction * (collisionDistance - pointDistance1);
		myNPC->setPos(New_Position);

		// Calculating new velocity to make npc bounce back.
		Vec3 New_Velocity = New_direction * VMath::mag(myNPC->getVel());
		myNPC->setVel(New_Velocity);
	}
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Render the background
	SDL_Rect bgRect;
	bgRect.x = 0;
	bgRect.y = 0;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	bgRect.w = w;
	bgRect.h = h;
	//SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);

	// Render walls
	for (Wall* wall : walls) {
		wall->Render();
	}

	// render any npc's
	Enemy1->render(5.15f);

	renderMyNPC();
	// render the player
	game->RenderPlayer(5.10f);

	SDL_RenderPresent(renderer);
}
void Scene1::renderMyNPC()
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

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed

	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
