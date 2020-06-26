#include "game.h"
#include "render.h"
#include "Engine/pallete.h"
#include "Engine/input.h"


const int PALLET_WIDTH = 5;
const int PALLET_HEIGHT = 40;


void Pallet_Init(Game* game)
{
    int palletDistance = 50;

    game->palletA.position.x = game->fieldRight - palletDistance - PALLET_WIDTH;
    game->palletB.position.x = game->fieldLeft + palletDistance;

    game->palletA.position.y = game->fieldTop +
                               (game->fieldBottom - game->fieldTop)/2.0f
                                - PALLET_HEIGHT/2.0f;
    game->palletB.position.y = game->palletA.position.y;

    game->palletA.speed = 0.0f;
    game->palletB.speed = 0.0f;

    game->palletA.score = 0;
    game->palletB.score = 0;
}

void Move(Game* game, Pallet* pallet)
{
    Vec2 velocity = Vector2(0.0f, pallet->speed);
    pallet->position = Vector2_Add(pallet->position, velocity);

    // Lose velocity due to friction (smooth movement)
    pallet->speed *= 0.9f;

    // Don't go out of the play field
    if (pallet->position.y < game->fieldTop)
        pallet->position.y = game->fieldTop;
    if (pallet->position.y + PALLET_HEIGHT > game->fieldBottom)
        pallet->position.y = game->fieldBottom - PALLET_HEIGHT;
}

void Pallet_Update(Game* game)
{
    Input* input = game->input;
    Pallet* a = &game->palletA;
    Pallet* b = &game->palletB;

    const float PALLET_SPEED = 8.0f;

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

    // Move
    Move(game, a);
    Move(game, b);
}

void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet)
{
    SDL_Rect rect;
    rect.x = (int)pallet->position.x;
    rect.y = (int)pallet->position.y;
    rect.w = PALLET_WIDTH;
    rect.h = PALLET_HEIGHT;

    Render_SetDrawColor(renderer, &white);
    SDL_RenderFillRect(renderer, &rect);
}