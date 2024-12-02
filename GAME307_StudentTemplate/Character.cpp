#include "Character.h"
#include "PlayerInRange.h"

bool Character::OnCreate(Scene* scene_)
{
	scene = scene_;

	// Configure and instantiate the body to use for the demo
	if (!body)
	{
		float radius = 0.7;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = 3.0f;
		float maxAcceleration = 10.0f;
		float maxRotation = 2.0f;
		float maxAngular = 10.0f;
		body = new KinematicBody(
			Vec3(10.0f, 5.0f, 0), Vec3(0, 0, 0), Vec3(0, 0, 0), 1.0f,
			radius,
			orientation,
			rotation,
			angular,
			maxSpeed,
			maxAcceleration,
			maxRotation,
			maxAngular
		);
	}

	if (!body)
	{
		return false;
	}

	return true;
}

void Character::OnDestroy()
{
	if (body)
	{
		if (body->getTexture())
		{
			SDL_DestroyTexture(body->getTexture());
		}
		delete body;
	}
};

bool Character::setTextureWith(string file)
{
	SDL_Surface* image = IMG_Load(file.c_str());
	if (image == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	SDL_Window* window = scene->getWindow();
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	if (!texture)
	{
		std::cerr << "Can't create texture" << std::endl;
		return false;
	}
	if (body->getTexture()) SDL_DestroyTexture(body->getTexture());
	body->setTexture(texture);
	SDL_FreeSurface(image);
	return true;
}

void Character::Update(float deltaTime)
{
	// Create a new overall steering output
	SteeringOutput* steering = new SteeringOutput();

	// Get the current position and velocity of the character
	Vec3 position = body->getPos();
	Vec3 velocity = body->getVel();

	// Get scene boundaries
	float radius = body->getRadius();
	float minX = radius;
	float maxX = scene->game->getSceneWidth() - radius;
	float minY = radius;
	float maxY = scene->game->getSceneHeight() - radius;

	// Boundary checks and position correction
	if (position.x < minX) {
		position.x = minX;
		velocity.x = std::abs(velocity.x); // Rebound right
	}
	else if (position.x > maxX) {
		position.x = maxX;
		velocity.x = -std::abs(velocity.x); // Rebound left
	}

	if (position.y < minY) {
		position.y = minY;
		velocity.y = std::abs(velocity.y); // Rebound up
	}
	else if (position.y > maxY) {
		position.y = maxY;
		velocity.y = -std::abs(velocity.y); // Rebound down
	}

	// Apply seeking behavior even after collisions
	if (decisionTree) {
		DecisionTreeNode* action = decisionTree->makeDecision();

		switch (static_cast<Action*>(action)->getValue()) {
		case ACTION_SET::SEEK:
			steerToSeekPlayer(steering); // Continue seeking
			break;

		case ACTION_SET::DO_NOTHING:
			// Stop movement if no action
			steering->linear = Vec3(0.0f, 0.0f, 0.0f);
			steering->angular = 0.0f;
			break;

		default:
			break;
		}
	}

	// Apply steering and position updates
	body->setPos(position);
	body->setVel(velocity);
	body->Update(deltaTime, steering);

	// Clean up steering output
	if (steering)
		delete steering;
}
void Character::steerToSeekPlayer(SteeringOutput* steering)
{
	// set the target for steering; target is used by the steerTo... functions
// (often the target is the Player)

// using the target, calculate and set values in the overall steering output
// TODO: error handling if new fails

	SteeringBehaviour* steering_algorithm = new Seek(body, scene->game->getPlayer());
	*steering += *(steering_algorithm->getSteering());


	// clean up memory
	// (delete only those objects created in this function)
	if (steering_algorithm)
	{
		delete steering_algorithm;
	}
}
void Character::HandleEvents(const SDL_Event& event)
{
	// handle events here, if needed
}

void Character::render(const Vec3& cameraOffset, float scale) const
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	// Adjust position by subtracting the camera offset
	Vec3 adjustedPos = body->getPos() - cameraOffset;

	// notice use of "body" in the following
	SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);
	w = static_cast<int>(w * scale);
	h = static_cast<int>(h * scale);
	screenCoords = projectionMatrix * adjustedPos;
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = w;
	square.h = h;

	// Convert character orientation from radians to degrees.
	float orientation = body->getOrientation() * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, body->getTexture(), nullptr, &square,
		orientation, nullptr, SDL_FLIP_NONE);
}

bool Character::readDecisionTreeFromFile(string file_)
{
	// I'm faking it!!!!
	// Here's where I would read XML from some file
	// TODO: read XML

	if (file_ == "blinky")
	{
		decisionTree = new Action(ACTION_SET::SEEK);
		return true;
	}
	else if (file_ == "nearPlayer")
	{
		DecisionTreeNode* trueNode = new Action(ACTION_SET::SEEK);
		DecisionTreeNode* falseNode = new Action(ACTION_SET::DO_NOTHING);
		decisionTree = new PlayerInRange(trueNode, falseNode, this);
		return true;

	}
	return false;
}

