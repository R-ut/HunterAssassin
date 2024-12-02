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

void Wall::Render(const Matrix4& projectionMatrix, const Vec3& cameraOffset) {
    // Adjust the wall position by subtracting the camera offset
    Vec3 adjustedPos = position - cameraOffset;
    // Project wall position using the projection matrix
    Vec3 screenCoords = projectionMatrix * adjustedPos;
    // Define the rectangle for rendering with increased scaling for visibility
    SDL_Rect wallRect = {
        static_cast<int>(screenCoords.x),
        static_cast<int>(screenCoords.y),
        static_cast<int>(width * 25),  // Increased width for better visibility
        static_cast<int>(height * 25)  // Increased height for better visibility
    };
    // Temporary color for testing
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for visibility
    SDL_RenderFillRect(renderer, &wallRect);  // Fill rectangle to visualize position and size
   // SDL_RenderCopy(renderer, texture, nullptr, &wallRect);
}