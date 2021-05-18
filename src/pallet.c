#include "Engine/input.h"
#include "Engine/pallete.h"
#include "Engine/vector.h"
#include "game.h"
#include "render.h"

const int PALLET_WIDTH  = 5;
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

    a->position.y = game->fieldMiddle.y - PALLET_HEIGHT / 2.0f;
    b->position.y = a->position.y;

    a->orientation = Vector2(-1.0f, 0.0f);
    b->orientation = Vector2(1.0f, 0.0f);

    a->speed = b->speed = 0.0f;

    a->score = b->score = 0;
}

// Keeps a pallet from going out of the playing field
static void LimitMovement(Game* game, Pallet* pallet)
{
    if (pallet->position.y < game->fieldTop)
        pallet->position.y = game->fieldTop;
    if (pallet->position.y + PALLET_HEIGHT > game->fieldBottom)
        pallet->position.y = game->fieldBottom - PALLET_HEIGHT;
}

// Kinematics!
static void Move(Game* game, Pallet* pallet)
{
    Vec2 velocity    = Vector2(0.0f, pallet->speed * game->dt);
    pallet->position = Vector2_Add(pallet->position, velocity);
}

// Deals with input of a pallet
static void HandleInput(Pallet* pallet, SDL_Scancode moveUpKey, SDL_Scancode moveDownKey)
{
    const float PALLET_SPEED = 400.0f;

    if (IsKeyDown(moveUpKey)) {
        pallet->speed = -PALLET_SPEED;
    } else if (IsKeyDown(moveDownKey)) {
        pallet->speed = PALLET_SPEED;
    } else if (IsKeyReleased(moveUpKey) || IsKeyReleased(moveDownKey)) {
        pallet->speed = 0;
    }
}

void Pallet_Update(Game* game)
{
    Pallet* a = &game->palletA;
    Pallet* b = &game->palletB;

    HandleInput(a, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);
    HandleInput(b, SDL_SCANCODE_A, SDL_SCANCODE_Z);

    Move(game, a);
    Move(game, b);

    LimitMovement(game, a);
    LimitMovement(game, b);
}

void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet)
{
    DrawFillRect(renderer, &white, pallet->position, PALLET_WIDTH, PALLET_HEIGHT);
}
