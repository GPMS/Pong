#include "game.h"
#include "render.h"
#include "Engine/pallete.h"
#include "Engine/input.h"
#include "Engine/vector.h"


const int PALLET_WIDTH = 5;
const int PALLET_HEIGHT = 60;


void Pallet_Init(Game* game)
{
    // Distance from the borders of the
    // playing field
    int palletDistance = 50;

    Pallet* a = &game->palletA;
    Pallet* b = &game->palletB;

    a->position.x = game->fieldRight - palletDistance - PALLET_WIDTH;
    b->position.x = game->fieldLeft + palletDistance;

    a->position.y = game->fieldMiddle.y - PALLET_HEIGHT/2.0f;
    b->position.y = a->position.y;

    a->orientation = Vector2(-1.0f, 0.0f);
    b->orientation = Vector2(1.0f, 0.0f);

    a->speed = b->speed = 0.0f;

    a->score = b->score = 0;
}

// Keeps a pallet from going out of the playing field
void LimitMovement(Game* game, Pallet* pallet)
{
    if (pallet->position.y < game->fieldTop)
        pallet->position.y = game->fieldTop;
    if (pallet->position.y + PALLET_HEIGHT > game->fieldBottom)
        pallet->position.y = game->fieldBottom - PALLET_HEIGHT;
}

// Kinematics!
void Move(Game* game, Pallet* pallet)
{
    Vec2 velocity = Vector2(0.0f, pallet->speed);
    velocity = Vector2_Mul(velocity, game->dt);

    pallet->position = Vector2_Add(pallet->position, velocity);

    // Lose velocity due to friction (smooth movement)
    pallet->speed *= 0.9f;
}

// Deals with input to the pallets
void HandleInput(Game* game)
{
    Input* input = game->input;
    Pallet* a = &game->palletA;
    Pallet* b = &game->palletB;

    const float PALLET_SPEED = 400.0f;

    if (Input_KeyWasPressed(input, SDLK_UP) ||
        Input_KeyIsBeingHeld(input, SDLK_UP))
    {
        a->speed = -PALLET_SPEED;
    }
    else if (Input_KeyWasPressed(input, SDLK_DOWN) ||
             Input_KeyIsBeingHeld(input, SDLK_DOWN))
    {
        a->speed = PALLET_SPEED;
    }

    if (Input_KeyWasPressed(input, SDLK_a) ||
        Input_KeyIsBeingHeld(input, SDLK_a))
    {
        b->speed = -PALLET_SPEED;
    }
    else if (Input_KeyWasPressed(input, SDLK_z) ||
             Input_KeyIsBeingHeld(input, SDLK_z))
    {
        b->speed = PALLET_SPEED;
    }
}

void Pallet_Update(Game* game)
{
    Pallet* a = &game->palletA;
    Pallet* b = &game->palletB;

    HandleInput(game);

    Move(game, a);
    Move(game, b);

    LimitMovement(game, a);
    LimitMovement(game, b);
}

void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet)
{
    DrawFillRect(renderer, &white,
                 pallet->position, PALLET_WIDTH, PALLET_HEIGHT);
}