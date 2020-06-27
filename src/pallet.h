#pragma once

#include "Engine/vector.h"
#include <SDL2/SDL.h>

extern const int PALLET_WIDTH;
extern const int PALLET_HEIGHT;

struct Game;

typedef struct Pallet {
    Vec2 position;
    Vec2 orientation;   // Direction the pallet is facing
    float speed;
    int score;
} Pallet;


// Inits both pallets with default values
void Pallet_Init(struct Game* game);

// Updates the position of both pallets
void Pallet_Update(struct Game* game);

// Draws one pallet
void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet);