#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "Scene.h"
#include "KinematicBody.h"
#include "Seek.h"
#include "Evade.h"
#include "Pursue.h"
#include "Arrive.h"
#include "Action.h"
#include "tinyxml2.h"
#include "Flee.h"

using namespace std;
using namespace tinyxml2;

class Character {
private:

    KinematicBody* body;     // Character's kinematic body
    Scene* scene;            // Scene the character is part of
    DecisionTreeNode* decisionTree;  // Decision tree for AI behavior

    // Steering behaviors
    void steerToSeekPlayer(SteeringOutput* steering);
    void steerToEvadePlayer(SteeringOutput* steering);
    void steerToPursuePlayer(SteeringOutput* steering);
    void steerToArrivePlayer(SteeringOutput* steering);

    void steerToFleePlayer(SteeringOutput* steering);

    // Position, velocity, and radius for rendering and movement
    Vec3 pos;
    Vec3 vel;
    float radius;
    float scale;

public:
    // Constructor and Destructor
    Character() : body(nullptr), scene(nullptr), decisionTree(nullptr), radius(1.0f), scale(1.0f) {}
    ~Character() {}

    // OnCreate and OnDestroy
    bool OnCreate(Scene* scene_);
    void OnDestroy();

    // Texture and Decision Tree handling
    bool setTextureWith(const std::string& file);
    bool readDecisionTreeFromFile(const std::string& file_);
    DecisionTreeNode* AnalyzeDecisionTreeNode(XMLElement* element);
  
    // Getters and Setters
    Vec3 getPos() const { return body->getPos(); }
    Vec3 getPlayerPos() const { return scene->game->getPlayer()->getPos(); }
    KinematicBody* getBody() const { return body; }
    void setPos(const Vec3& newPos) { pos = newPos; }
    Vec3 getVel() const { return vel; }
    void setVel(const Vec3& newVel) { vel = newVel; }
    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }
    float getScale() const { return scale; }
    float getHealth() const { return body->getHealth(); }
    void reduceHealth(float health_) { body->reduceHealth(health_); }
    void setScale(float newScale) { scale = newScale; }
    // Update function
    void Update(float time);

    // Event handling and rendering
    void HandleEvents(const SDL_Event& event);
    void render(const Vec3& cameraOffset, float scale, const Matrix4& projectionMatrix, bool useCameraOffset) const;
    //void render(const Vec3& cameraOffset, float scale = 1.0f) const;
};

#endif

