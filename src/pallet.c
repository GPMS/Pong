#include "Engine/input.h"
#include "Engine/pallete.h"
#include "Engine/vector.h"
#include "game.h"
#include "render.h"

const int PALLET_WIDTH  = 5;
const int PALLET_HEIGHT = 60;

void Pallet_Init(Pallet*      pallet,
                 Vec2         position,
                 Vec2         orientation,
                 SDL_Scancode moveUpKey,
                 SDL_Scancode moveDownKey)
{
    pallet->position    = position;
    pallet->orientation = orientation;
    pallet->moveUpKey   = moveUpKey;
    pallet->moveDownKey = moveDownKey;
    pallet->speed       = 0.0f;
    pallet->score       = 0;
}

// Keeps a pallet from going out of the playing field
static void LimitMovement(Pallet* pallet, int maxY, int minY)
{
    if (pallet->position.y < maxY)
        pallet->position.y = maxY;
    if (pallet->position.y + PALLET_HEIGHT > minY)
        pallet->position.y = minY - PALLET_HEIGHT;
}

// Kinematics!
static void Move(Pallet* pallet, float dt)
{
    Vec2 velocity    = Vector2(0.0f, pallet->speed * dt);
    pallet->position = Vector2_Add(pallet->position, velocity);
}

// Deals with input of a pallet
static void HandleInput(Pallet* pallet)
{
    const float PALLET_SPEED = 400.0f;

    if (IsKeyDown(pallet->moveUpKey)) {
        pallet->speed = -PALLET_SPEED;
    } else if (IsKeyDown(pallet->moveDownKey)) {
        pallet->speed = PALLET_SPEED;
    } else if (IsKeyReleased(pallet->moveUpKey) || IsKeyReleased(pallet->moveDownKey)) {
        pallet->speed = 0;
    }
}

void Pallet_Update(Pallet* pallet, float dt)
{
    HandleInput(pallet);
    Move(pallet, dt);
    LimitMovement(pallet, FIELD_TOP, FIELD_BOTTOM);
}

void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet)
{
    DrawFillRect(renderer, &white, pallet->position, PALLET_WIDTH, PALLET_HEIGHT);
}
