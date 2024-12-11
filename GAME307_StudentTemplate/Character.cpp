#include "Character.h"
#include "PlayerInRange.h"
#include "PlayerCloseRange.h"
#include "PlayerFarRange.h"
#include "tinyxml2.h"

using namespace tinyxml2;
bool Character::OnCreate(Scene* scene_) {
    scene = scene_;

    if (!body) {
        // Initialize the kinematic body with proper parameters
        float radius = 0.7f;
        float orientation = 0.0f;
        float rotation = 0.0f;
        float angular = 0.0f;
        float maxSpeed = 5.0f;
        float maxAcceleration = 10.0f;
        float maxRotation = 2.0f;
        float maxAngular = 10.0f;

        body = new KinematicBody(
            Vec3(10.0f, 5.0f, 0), Vec3(0, 0, 0), Vec3(0, 0, 0), 1.0f,
            radius, orientation, rotation, angular,
            maxSpeed, maxAcceleration, maxRotation, maxAngular
        );
    }

    if (!body) {
        std::cerr << "Error: Failed to create body.\n";
        return false;
    }

    return true;
}

void Character::OnDestroy() {
    if (body) {
        if (body->getTexture()) {
            SDL_DestroyTexture(body->getTexture());
        }
        delete body;
    }
    // If decisionTree is dynamically allocated, clean up here
    if (decisionTree) {
        delete decisionTree;
    }
}

bool Character::setTextureWith(const std::string& file) {
    SDL_Surface* image = IMG_Load(file.c_str());
    if (!image) {
        std::cerr << "Can't open the image: " << file << std::endl;
        return false;
    }

    SDL_Window* window = scene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        std::cerr << "Can't create texture from image: " << file << std::endl;
        SDL_FreeSurface(image);
        return false;
    }

    if (body->getTexture()) {
        SDL_DestroyTexture(body->getTexture());
    }

    body->setTexture(texture);
    SDL_FreeSurface(image);
    return true;
}

void Character::steerToSeekPlayer(SteeringOutput* steering) {
    SteeringBehaviour* steering_algorithm = new Seek(body, scene->game->getPlayer());
    *steering += *(steering_algorithm->getSteering());
    delete steering_algorithm;
}


void Character::steerToEvadePlayer(SteeringOutput* steering) {
    SteeringBehaviour* steering_algorithm = new Evade(body, scene->game->getPlayer(), 2.0f);  // Adjust maxPrediction if needed
    *steering += *(steering_algorithm->getSteering());
    delete steering_algorithm;
}

void Character::steerToPursuePlayer(SteeringOutput* steering) {
    SteeringBehaviour* steering_algorithm = new Pursue(body, scene->game->getPlayer(), 2.0f);  // Adjust maxPrediction if needed
    *steering += *(steering_algorithm->getSteering());
    delete steering_algorithm;
}

void Character::steerToArrivePlayer(SteeringOutput* steering) {
    SteeringBehaviour* steering_algorithm = new Arrive(body, scene->game->getPlayer());
    *steering += *(steering_algorithm->getSteering());
    delete steering_algorithm;
}

void Character::steerToFleePlayer(SteeringOutput* steering) {
    // Create a Flee behavior, passing the body and the player as the target
    SteeringBehaviour* steering_algorithm = new Flee(body, scene->game->getPlayer());
    *steering += *(steering_algorithm->getSteering());
    delete steering_algorithm;
}





void Character::Update(float deltaTime) {
    SteeringOutput* steering = new SteeringOutput();

    // Apply decision tree logic for behavior
    if (decisionTree) {
        DecisionTreeNode* action = decisionTree->makeDecision();

        switch (static_cast<Action*>(action)->getValue()) {
        case ACTION_SET::SEEK:
            steerToSeekPlayer(steering);
            break;
        case ACTION_SET::EVADE:
            steerToEvadePlayer(steering);
            break;
        case ACTION_SET::PURSUE:
            steerToPursuePlayer(steering);
            break;
        case ACTION_SET::ARRIVE:
            steerToArrivePlayer(steering);
            break;
        case ACTION_SET::FLEE:
			steerToFleePlayer(steering);
			break;
        case ACTION_SET::DO_NOTHING:
            steering->linear = Vec3(0.0f, 0.0f, 0.0f);
            //steering->angular = 0.0f;
            break;
        default:
            break;
        }
    }


    // Update velocity with steering output
    Vec3 velocity = body->getVel();
    velocity += steering->linear * deltaTime;

    // Clamp velocity to max speed
    if (VMath::mag(velocity) > body->getMaxSpeed()) {
        velocity = VMath::normalize(velocity) * body->getMaxSpeed();
    }

    // Update position with velocity
    Vec3 position = body->getPos();
    position += velocity * deltaTime;

    // Apply updates
    body->setPos(position);
    body->setVel(velocity);

    // Clean up steering output
    delete steering;
}



void Character::HandleEvents(const SDL_Event& event)
{
}

void Character::render(const Vec3& cameraOffset, float scale, const Matrix4& projectionMatrix, bool useCameraOffset) const {
    SDL_Renderer* renderer = scene->game->getRenderer();
    SDL_Rect square;
    Vec3 screenCoords;
    int w, h;

    // Retrieve the texture dimensions
    SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);

    // Apply scaling to the texture dimensions
    w = static_cast<int>(w * scale);
    h = static_cast<int>(h * scale);

    // Adjust position manually based on the flag
    Vec3 adjustedPos = body->getPos();
    if (useCameraOffset) {
        adjustedPos -= cameraOffset;
    }

    // Project the adjusted position to screen coordinates
    screenCoords = projectionMatrix * Vec4(adjustedPos.x, adjustedPos.y, adjustedPos.z, 1.0f);

    // Set up the rectangle for rendering
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);  // Center the texture horizontally
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);  // Center the texture vertically
    square.w = w;
    square.h = h;

    // Render the character with the texture
    float orientation = body->getOrientation() * 180.0f / M_PI;  // Convert radians to degrees
    SDL_RenderCopyEx(renderer, body->getTexture(), nullptr, &square, orientation, nullptr, SDL_FLIP_NONE);
}



bool Character::readDecisionTreeFromFile(const std::string& file_) {
    // Load the XML file
    XMLDocument doc;

    // If the file fails to load, print an error message and return false
    if (doc.LoadFile(file_.c_str()) != XML_SUCCESS) {
        std::cout << "Error loading XML file: " << file_ << std::endl;
        return false;
    }

    // Analyze the root element of the XML file
    XMLElement* root = doc.RootElement();
    // Analyze the decision tree continuesly starting from the root element
    decisionTree = AnalyzeDecisionTreeNode(root);
    return decisionTree != nullptr;
}

DecisionTreeNode* Character::AnalyzeDecisionTreeNode(XMLElement* element) {
    if (!element) {
        return nullptr;
    }

    // Retrieve the 'type' attribute of the current node element
    std::string type = element->Attribute("type");

    // Debugging output if no 'type' attribute is found
    if (type.empty()) {
        std::cout << "Error: Missing 'type' attribute in node." << std::endl;
        return nullptr;
    }

    // If the node type is "Action", put it in Action node
    if (type == "Action") {
        // Retrieve the 'value' attribute from the Action node
        const char* valueStr = element->Attribute("value");
        // Convert the string value to an integer and map it to an ACTION_SET value
        ACTION_SET actionValue = static_cast<ACTION_SET>(std::stoi(valueStr));
       
        return new Action(actionValue);

    }
    else if (type == "PlayerInRange") {
        // Get the TrueNode and FalseNode child elements for this PlayerInRange node
        XMLElement* trueNodeElement = element->FirstChildElement("TrueNode");
        XMLElement* falseNodeElement = element->FirstChildElement("FalseNode");

        // Analyze the TrueNode and FalseNode elements
        DecisionTreeNode* trueNode = AnalyzeDecisionTreeNode(trueNodeElement);
        DecisionTreeNode* falseNode = AnalyzeDecisionTreeNode(falseNodeElement);

        return new PlayerInRange(trueNode, falseNode, this);
    }

    else if (type == "PlayerCloseRange") {
        XMLElement* trueNodeElement = element->FirstChildElement("TrueNode");
        XMLElement* falseNodeElement = element->FirstChildElement("FalseNode");

        DecisionTreeNode* trueNode = AnalyzeDecisionTreeNode(trueNodeElement);
        DecisionTreeNode* falseNode = AnalyzeDecisionTreeNode(falseNodeElement);

        return new PlayerCloseRange(trueNode, falseNode, this);
    }

    else if (type == "PlayerFarRange") {
        XMLElement* trueNodeElement = element->FirstChildElement("TrueNode");
        XMLElement* falseNodeElement = element->FirstChildElement("FalseNode");

        DecisionTreeNode* trueNode = AnalyzeDecisionTreeNode(trueNodeElement);
        DecisionTreeNode* falseNode = AnalyzeDecisionTreeNode(falseNodeElement);

        return new PlayerFarRange(trueNode, falseNode, this);
    }
}



