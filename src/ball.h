#pragma once

#include "Engine/vector.h"
#include <SDL2/SDL.h>

extern const int BALL_SIZE;

struct Game;

typedef struct PosList {
    Vec2            position;
    struct PosList* next;
    struct PosList* prev;
} PosList;

typedef struct Ball {
    Vec2 position;

    PosList* trail;
    PosList* trailLast;
    int      trailSize;
    int      trailCapacity;

    Vec2  direction;
    float speed;
} Ball;

// Resets ball parameters to default
void Ball_Reset(Ball* ball, int turn);

// Updates the ball position, checking for collision
void Ball_Update(struct Game* game);

// Draws the ball alongside its trail
void Ball_Draw(SDL_Renderer* renderer, Ball* ball);
