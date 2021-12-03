#include "Engine/pallete.h"
#include "game.h"
#include "render.h"
#include <stdlib.h>
#include <stdbool.h>

const int   BALL_SIZE        = 10;      // "Ball" is a square, this is its size
const float INITIAL_SPEED    = 100.0f;  // Initial speed of the ball
const float MAX_SPEED        = 500.0f;  // Max speed the ball can reach
const int   INITIAL_MAX_SIZE = 30.0f;   // Initial max size of the trail

static bool IsBallColliding(Ball* ball, Pallet* pallet)
{
    const int ballTop    = ball->position.y;
    const int ballBottom = ball->position.y + BALL_SIZE;
    const int ballLeft   = ball->position.x;
    const int ballRight  = ball->position.x + BALL_SIZE;

    const int palletTop    = pallet->position.y;
    const int palletBottom = pallet->position.y + PALLET_HEIGHT;
    const int palletLeft   = pallet->position.x;
    const int palletRight  = pallet->position.x + PALLET_WIDTH;

    return (ballLeft < palletRight
            && ballRight > palletLeft
            && ballTop < palletBottom
            && ballBottom > palletTop);
}

// Handles ball collision with a pallet
static void PalletCollision(Ball* ball, Pallet* pallet)
{
    if (IsBallColliding(ball, pallet)) {
        // Adjust position
        if (pallet->orientation.x == 1.0f)
            ball->position.x = pallet->position.x + PALLET_WIDTH + 1;
        else
            ball->position.x = pallet->position.x - BALL_SIZE - 1;

        // Reflect
        ball->direction.x *= -1.0f;

        // Adjust ball direction depending on which
        // part of the pallet the ball has hit
        if (pallet->orientation.y > pallet->position.y + PALLET_HEIGHT / 2.0f)
            ball->direction.y += 0.5f;
        else
            ball->direction.y -= 0.5f;

        // Increase the speed
        ball->speed += 20.0f;
        if (ball->speed > MAX_SPEED)
            ball->speed = MAX_SPEED;

        // Change size of trail based on speed
        ball->trailCapacity = Lerp(INITIAL_MAX_SIZE, 100, (ball->speed - INITIAL_SPEED) / (float)MAX_SPEED);
    }
}

// Returns a direction pointing to a random angle
// according to whose turn it currently is
static Vec2 GetRandDirection(int turn)
{
    Vec2 direction;

    // Left or Right?
    if (turn == 0)
        direction = Vector2(1.0f, 0.0f);
    else
        direction = Vector2(-1.0f, 0.0f);

    // Get random angle
    int randomAngle = rand() % 60;
    int isDown      = rand() % 2;
    randomAngle     = isDown ? -randomAngle : randomAngle;

    // Return direction based on angle
    return Vector2_Rotate(direction, DegreesToRadians(randomAngle));
}

// Clears memory from the trail's list of positions
static void EmptyList(Ball* ball)
{
    if (ball->trail != NULL) {
        PosList* current = ball->trail;
        while (current != NULL) {
            PosList* next = current->next;
            free(current);
            current = next;
        }
        ball->trail     = NULL;
        ball->trailLast = NULL;
    }
    ball->trailSize     = 0;
    ball->trailCapacity = INITIAL_MAX_SIZE;
}

void Ball_Reset(Ball* ball, int turn)
{
    ball->position  = FIELD_MIDDLE;
    ball->speed     = INITIAL_SPEED;
    ball->direction = GetRandDirection(turn);
    EmptyList(ball);
}

// Handles ball collision with the boundaries
// of the playing field
static void WallColision(Game* game)
{
    Ball*   ball    = &game->ball;
    Pallet* palletA = &game->palletA;
    Pallet* palletB = &game->palletB;

    // Boundaries of the ball

    int ballTop    = ball->position.y;
    int ballBottom = ball->position.y + BALL_SIZE;
    int ballRight  = ball->position.x + BALL_SIZE;
    int ballLeft   = ball->position.x;

    // Up/Down collision, reflect...

    if (ballTop < FIELD_TOP) {
        ball->position.y = FIELD_TOP;
        ball->direction.y *= -1.0f;
    }
    if (ballBottom > FIELD_BOTTOM) {
        ball->position.y = FIELD_BOTTOM - BALL_SIZE;
        ball->direction.y *= -1.0f;
    }

    // Was a point scored?

    bool hasScored = false;

    if (ballRight > FIELD_RIGHT) {
        palletB->score++;
        hasScored = true;
    } else if (ballLeft < FIELD_LEFT) {
        palletA->score++;
        hasScored = true;
    }

    if (hasScored) {
        game->currentSet++;
        if (game->currentSet > game->totalSets)
            game->state = END;
        else
            Ball_Reset(ball, game->currentSet % 2);
    }
}

// Returns a new node for a positions list
static PosList* NewPos(Vec2 position, PosList* next)
{
    PosList* new  = calloc(1, sizeof(PosList));
    new->position = position;
    new->next     = next;
    new->prev     = NULL;

    if (next) {
        next->prev = new;
    }

    return new;
}

// Add a position to the trail
static void AddPos(Ball* ball, Vec2 position)
{
    ball->trailSize++;

    PosList* new = NewPos(position, ball->trail);
    ball->trail  = new;

    if (!new->next) {
        ball->trailLast = new;
        return;
    }

    // Max size exceeded? delete oldest node
    if (ball->trailSize > ball->trailCapacity) {
        PosList* deleteMe = ball->trailLast;
        ball->trailLast   = ball->trailLast->prev;
        free(deleteMe);

        ball->trailLast->next = NULL;
        ball->trailSize--;
    }
}

void Ball_Update(Game* game)
{
    Ball*   ball    = &game->ball;
    Pallet* palletA = &game->palletA;
    Pallet* palletB = &game->palletB;

    AddPos(ball, ball->position);

    // Change position based on speed and direction (velocity)
    Vec2 velocity  = Vector2_Mul(ball->direction, ball->speed);
    velocity       = Vector2_Mul(velocity, game->dt);
    ball->position = Vector2_Add(ball->position, velocity);

    // Handle collision
    WallColision(game);
    PalletCollision(ball, palletA);
    PalletCollision(ball, palletB);
}

static void DrawTrail(SDL_Renderer* renderer, const SDL_Color* color, Ball* ball)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    PosList* current = ball->trail;
    for (int index = 0; current; ++index) {
        // color gets more and more transparent as the trail goes
        SDL_Color newColor = *color;
        newColor.a -= Lerp(200.0f, 250.0f, index / (float)ball->trailCapacity);

        DrawFillRect(renderer, &newColor, current->position, BALL_SIZE, BALL_SIZE);

        current = current->next;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Ball_Draw(SDL_Renderer* renderer, Ball* ball)
{
    DrawFillRect(renderer, &white, ball->position, BALL_SIZE, BALL_SIZE);
    DrawTrail(renderer, &white, ball);
}
