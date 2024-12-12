//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"
#include "GameManager.h"
bool PlayerBody::OnCreate()
{
    image = IMG_Load( "Pacman.png" );
    if (image == nullptr) {
        std::cerr << "Can't open the image" << std::endl;
        return false;
    }
    SDL_Renderer *renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface( renderer, image );
    if (!texture)
    {
        std::cerr << "Can't create the texture" << std::endl;
        return false;
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
        return false;
    }

    //SDL_Joystick* joystick = nullptr; // Pointer to joystick
    //// Enable joystick events
    //SDL_JoystickEventState(SDL_ENABLE);

    //// Check for joysticks
    //if (SDL_NumJoysticks() > 0) {
    //    joystick = SDL_JoystickOpen(0); // Open the first joystick
    //}
    return true;
}

void PlayerBody::OnDestroy()
{
    if (image) SDL_FreeSurface(image);
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void PlayerBody::Render( float scale ) const
{
    // This is why we need game in the constructore, to get the renderer, etc.
    SDL_Renderer *renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    // square represents the position and dimensions for where to draw the image
    SDL_Rect square;
    Vec3 screenCoords;
    float    w, h;

    // convert the position from game coords to screen coords
    screenCoords = projectionMatrix * pos;
    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of
    // where SDL will draw the .png image
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtractions!!!!)
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    // Convert character orientation from radians to degrees.
    float orientationDegrees = orientation * 180.0f / M_PI ;

    SDL_RenderCopyEx( renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE );
}

void PlayerBody::HandleEvents( const SDL_Event& event )
{
    // Handle key press and joystick events for velocity and acceleration
    if ((event.type == SDL_KEYDOWN && event.key.repeat == 0) || event.type == SDL_CONTROLLERAXISMOTION) {
        SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN; // Default to no scancode
        bool isJoystick = (event.type == SDL_CONTROLLERAXISMOTION);

        if (isJoystick) {
            const float deadzone = 8000; // Deadzone for joystick
            const float scale = 32767.0f; // Maximum joystick axis value

            switch (event.caxis.axis) {
            case SDL_CONTROLLER_AXIS_LEFTX: // Left joystick X-axis for velocity.x
                if (std::abs(event.caxis.value) > deadzone) {
                    vel.x = maxSpeed * (event.caxis.value / scale);
                }
                else {
                    vel.x = 0.0f; // Stop when within deadzone
                }
                break;

            case SDL_CONTROLLER_AXIS_LEFTY: // Left joystick Y-axis for velocity.y
                if (std::abs(event.caxis.value) > deadzone) {
                    vel.y = maxSpeed * -(event.caxis.value / scale); // Invert Y for proper control
                }
                else {
                    vel.y = 0.0f;
                }
                break;

            case SDL_CONTROLLER_AXIS_RIGHTX: // Right joystick X-axis for acceleration.x
                if (std::abs(event.caxis.value) > deadzone) {
                    accel.x = maxAcceleration * (event.caxis.value / scale);
                }
                else {
                    accel.x = 0.0f;
                }
                break;

            case SDL_CONTROLLER_AXIS_RIGHTY: // Right joystick Y-axis for acceleration.y
                if (std::abs(event.caxis.value) > deadzone) {
                    accel.y = maxAcceleration * -(event.caxis.value / scale); // Invert Y for proper control
                }
                else {
                    accel.y = 0.0f;
                }
                break;

            default:
                break;
            }
        }
        else {
            scancode = event.key.keysym.scancode;

            // Handle keyboard inputs for velocity and acceleration
            switch (scancode) {
                // Velocity controls
            case SDL_SCANCODE_W:
                vel.y = maxSpeed * 1.0f;
                break;
            case SDL_SCANCODE_A:
                vel.x = maxSpeed * -1.0f;
                break;
            case SDL_SCANCODE_S:
                vel.y = maxSpeed * -1.0f;
                break;
            case SDL_SCANCODE_D:
                vel.x = maxSpeed * 1.0f;
                break;

                // Acceleration controls
            case SDL_SCANCODE_DOWN:
                accel.y = maxAcceleration * -1.0f;
                break;
            case SDL_SCANCODE_UP:
                accel.y = maxAcceleration * 1.0f;
                break;
            case SDL_SCANCODE_LEFT:
                accel.x = maxAcceleration * -1.0f;
                break;
            case SDL_SCANCODE_RIGHT:
                accel.x = maxAcceleration * 1.0f;
                break;

            default:
                break;
            }
        }
    }

    // Handle key release events to stop movement
    if ((event.type == SDL_KEYUP && event.key.repeat == 0) || event.type == SDL_CONTROLLERAXISMOTION) {
        SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN; // Default to no scancode
        bool isJoystick = (event.type == SDL_CONTROLLERAXISMOTION);

        if (isJoystick) {
            const float deadzone = 8000; // Deadzone for joystick

            switch (event.caxis.axis) {
            case SDL_CONTROLLER_AXIS_LEFTX: // Left joystick X-axis for velocity.x
                if (std::abs(event.caxis.value) <= deadzone) {
                    vel.x = 0.0f; // Stop when within deadzone
                }
                break;

            case SDL_CONTROLLER_AXIS_LEFTY: // Left joystick Y-axis for velocity.y
                if (std::abs(event.caxis.value) <= deadzone) {
                    vel.y = 0.0f;
                }
                break;

            case SDL_CONTROLLER_AXIS_RIGHTX: // Right joystick X-axis for acceleration.x
                if (std::abs(event.caxis.value) <= deadzone) {
                    accel.x = 0.0f;
                }
                break;

            case SDL_CONTROLLER_AXIS_RIGHTY: // Right joystick Y-axis for acceleration.y
                if (std::abs(event.caxis.value) <= deadzone) {
                    accel.y = 0.0f;
                }
                break;

            default:
                break;
            }
        }
        else {
            scancode = event.key.keysym.scancode;

            // Stop keyboard movement
            switch (scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                vel.y = 0.0f;
                if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
                break;

            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                vel.x = 0.0f;
                if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
                break;

            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_UP:
                accel.y = 0.0;
                break;

            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_RIGHT:
                accel.x = 0.0;
                break;

            default:
                break;
            }
        }
    }
}

void PlayerBody::Update( float deltaTime )
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    Body::Update( deltaTime );

    // This will ensure player body stops at edges
    float height, width;
    height = game->getSceneHeight();
    width = game->getSceneWidth();
    
    if (pos.x < radius)
    {
        pos.x = radius;
        vel.x = 0.0f;
    }
    if (pos.y < radius)
    {
        pos.y = radius;
        vel.y = 0.0f;
    }
    if (pos.x > width-radius)
    {
        pos.x = width - radius;
        vel.x = 0.0f;
    }
    if (pos.y > height-radius)
    {
        pos.y = height - radius;
        vel.y = 0.0f;
    }
}

void PlayerBody::resetToOrigin()
{
    pos = Vec3( 0.0f + radius, 0.0f + radius, 0.0f );
}
