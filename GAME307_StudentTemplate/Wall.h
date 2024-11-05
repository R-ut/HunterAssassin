#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include "Scene.h"
#include "GameManager.h"

class Wall {
private:
    class GameManager* game;
    Vec3 position; // Position in world coordinates
    float width, height;
    SDL_Texture* texture; // Texture for the wall
    SDL_Renderer* renderer;

public:
    Wall(Vec3 pos, float w, float h, SDL_Renderer* renderer_);
    void Render();
    Vec3 getPosition() const { return position; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

#endif
