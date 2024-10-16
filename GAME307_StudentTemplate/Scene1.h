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
	

	KinematicBody* myNPC;
	Character* Enemy1;

public:

	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }

	void renderMyNPC();
};

#endif
