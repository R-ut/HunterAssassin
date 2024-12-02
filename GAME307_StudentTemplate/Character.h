#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "KinematicBody.h"

#include "Seek.h"

#include "Action.h"

using namespace std;

class StateMachine;

class Character
{
private:
	class KinematicBody* body;
	class Scene* scene;

	DecisionTreeNode* decisionTree;

	void steerToSeekPlayer(SteeringOutput* steering);

	Vec3 pos;   // Position of the character
	Vec3 vel; 
	float radius;

public:
	Character()
	{
		body = NULL;
		scene = NULL;
		decisionTree = NULL;

	}

	~Character() {};

	bool OnCreate(Scene* scene_);
	void OnDestroy();
	bool setTextureWith(string file);
	void Update(float time);
	void HandleEvents(const SDL_Event& event);
	void render(const Vec3& cameraOffset, float scale = 1.0f) const;
	bool readDecisionTreeFromFile(string file_);
	Vec3 getPos() const { return body->getPos(); }
	Vec3 getPlayerPos() const { return scene->game->getPlayer()->getPos(); }

	void setPos(const Vec3& newPos) { pos = newPos; }

	// Getter for velocity
	Vec3 getVel() const { return vel; }

	// Setter for velocity
	void setVel(const Vec3& newVel) { vel = newVel; }

	float getRadius() const { return radius; }
	void setRadius(float r) { radius = r; }
};

#endif

