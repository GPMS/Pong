#pragma once

#include "Engine/vector.h"
#include <SDL2/SDL.h>

extern const int BALL_SIZE;

struct Game;

typedef struct PosList {
    Vec2 position;
    struct PosList* next;
} PosList;

typedef struct Ball {
    Vec2 position;

    PosList* pastPositions;
    int listSize;

    Vec2 direction;
    float speed;
} Ball;


// Resets ball parameters to default
void Ball_Reset(struct Game* game, int turn);

// Updates the ball position, checking for collision
void Ball_Update(struct Game* game);

// Draws the ball alongside its trail
void Ball_Draw(SDL_Renderer* renderer, Ball* ball);