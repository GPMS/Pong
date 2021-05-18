#pragma once

#include "Engine/vector.h"
#include <SDL2/SDL.h>

extern const int PALLET_WIDTH;
extern const int PALLET_HEIGHT;

struct Game;

typedef struct Pallet {
    Vec2         position;
    Vec2         orientation;  // Direction the pallet is facing
    SDL_Scancode moveUpKey;
    SDL_Scancode moveDownKey;
    float        speed;
    int          score;
} Pallet;

void Pallet_Init(Pallet*      pallet,
                 Vec2         position,
                 Vec2         orientation,
                 SDL_Scancode moveUpKey,
                 SDL_Scancode moveDownKey);

void Pallet_Update(Pallet* pallet, float dt);

void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet);
