#pragma once

#include "Engine/vector.h"
#include <SDL2/SDL.h>

extern const int PALLET_WIDTH;
extern const int PALLET_HEIGHT;

struct Game;

typedef struct {
    Vec2 position;
    float speed;
    int score;
} Pallet;

void Pallet_Init(struct Game* game);
void Pallet_Update(struct Game* game);
void Pallet_Draw(SDL_Renderer* renderer, Pallet* pallet);