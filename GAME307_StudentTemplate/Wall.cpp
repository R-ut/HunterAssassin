#include "Wall.h"


Wall::Wall(Vec3 pos, float w, float h, SDL_Renderer* renderer_)
    : position(pos), width(w), height(h), renderer(renderer_) {
    // Load wall texture
    SDL_Surface* wallSurface = SDL_LoadBMP("tileset x1.png");
    if (wallSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, wallSurface);
        SDL_FreeSurface(wallSurface);
    }
    else {
        texture = nullptr;
    }
}

Wall::~Wall() {
    if (texture) SDL_DestroyTexture(texture);
}

void Wall::Render() {
    SDL_Rect wallRect = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(width),
        static_cast<int>(height)
    };
    // Temporary color for testing
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for visibility
    SDL_RenderFillRect(renderer, &wallRect);  // Fill rectangle to visualize position and size
   // SDL_RenderCopy(renderer, texture, nullptr, &wallRect);
}