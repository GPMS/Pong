#include "game.h"
#include "render.h"
#include "Engine/pallete.h"
#include <stdlib.h>

const int BALL_SIZE = 10;           // "Ball" is a square, this is its size
const float INITIAL_SPEED = 100.0f; // Initial speed of the ball
const float MAX_SPEED = 500.0f;     // Max speed the ball can reach
const int INITIAL_MAX_SIZE = 30.0f; // Initial max size of the trail
int MAX_SIZE;                       // Current max size of the trail


// Returns 1 if colliding, 0 otherwise
int PointRectCollision(Vec2 point, Pallet* pallet)
{
    if (point.x > pallet->position.x &&
        point.x < pallet->position.x + PALLET_WIDTH &&
        point.y > pallet->position.y &&
        point.y < pallet->position.y + PALLET_HEIGHT)
    {
        return 1;
    }
    return 0;
}

// Handles ball collision with a pallet
void PalletCollision(Game* game, Pallet* pallet)
{
    Ball* ball = &game->ball;

    // No collision is possible if the ball is going
    // in the direction the pallet is facing
    if (Vector2_Dot(ball->direction, pallet->orientation) > 0)
        return;

    Vec2 ballTopLeft = ball->position;
    Vec2 ballTopRight = Vector2_Add(ballTopLeft,
                                    Vector2(BALL_SIZE, 0.0f));
    Vec2 ballBottomLeft = Vector2_Add(ballTopLeft,
                                      Vector2(0.0f, BALL_SIZE));
    Vec2 ballBottomRight = Vector2_Add(ballTopLeft,
                                       Vector2(BALL_SIZE, BALL_SIZE));

    if (PointRectCollision(ballTopLeft, pallet) ||
        PointRectCollision(ballTopRight, pallet) ||
        PointRectCollision(ballBottomLeft, pallet) ||
        PointRectCollision(ballBottomRight, pallet))
    {
        // Adjust position
        if (pallet->orientation.x == 1.0f)
            ball->position.x = pallet->position.x + PALLET_WIDTH + 1;
        else
            ball->position.x = pallet->position.x - BALL_SIZE - 1;

        // Reflect
        ball->direction.x *= -1.0f;

        // Adjust ball direction depending on which
        // part of the pallet the ball has hit
        if (pallet->orientation.y > pallet->position.y + PALLET_HEIGHT/2.0f)
            ball->direction.y += 0.5f;
        else
            ball->direction.y -= 0.5f;

        // Increase the speed
        ball->speed += 20.0f;
        if (ball->speed > MAX_SPEED)
            ball->speed = MAX_SPEED;

        // Change size of trail based on speed
        MAX_SIZE = Lerp(INITIAL_MAX_SIZE, 100,
                        (ball->speed - INITIAL_SPEED) / (float)MAX_SPEED);
    }
}

// Returns a direction pointing to a random angle
// according to whose turn it currently is
Vec2 GetRandDirection(int turn)
{
    Vec2 direction;

    // Left or Right?
    if (turn == 0)
        direction = Vector2(1.0f, 0.0f);
    else
        direction = Vector2(-1.0f, 0.0f);

    // Get random angle
    int randomAngle = rand() % 60;
    int isDown = rand() % 2;
    randomAngle = isDown ? -randomAngle : randomAngle;

    // Return direction based on angle
    return Vector2_Rotate(direction, DegreesToRadians(randomAngle));
}

// Clears memory from the trail's list of positions
void EmptyList(Ball* ball)
{
    if (ball->pastPositions != NULL)
    {
        PosList* current = ball->pastPositions;
        while (current != NULL)
        {
            PosList* next = current->next;
            free(current);
            current = next;
        }
        ball->pastPositions = NULL;
    }
    ball->listSize = 0;
}

void Ball_Reset(Game* game, int turn)
{
    Ball* ball = &game->ball;

    if (game->currentSet > game->totalSets)
        game->state = END;

    ball->position = game->fieldMiddle;

    EmptyList(ball);

    ball->speed = INITIAL_SPEED;

    ball->direction = GetRandDirection(turn);

    MAX_SIZE = INITIAL_MAX_SIZE;
}

// Handles ball collision with the boundaries
// of the playing field
void WallColision(Game* game)
{
    Ball* ball = &game->ball;
    Pallet* palletA = &game->palletA;
    Pallet* palletB = &game->palletB;

    // Boundaries of the ball
    int ballTop = ball->position.y;
    int ballBottom = ball->position.y + BALL_SIZE;
    int ballRight = ball->position.x + BALL_SIZE;
    int ballLeft = ball->position.x;

    // Up/Down collision, reflect...
    if (ballTop < game->fieldTop)
    {
        ball->position.y = game->fieldTop;
        ball->direction.y *= -1.0f;
    }
    if (ballBottom > game->fieldBottom)
    {
        ball->position.y = game->fieldBottom - BALL_SIZE;
        ball->direction.y *= -1.0f;
    }

    // Was a point scored?
    if (ballRight > game->fieldRight)
    {
        palletB->score++;
        game->currentSet++;
        Ball_Reset(game, game->currentSet % 2);
    }
    else if (ballLeft < game->fieldLeft)
    {
        palletA->score++;
        game->currentSet++;
        Ball_Reset(game, game->currentSet % 2);
    }
}

// Returns a new node for a positions list
PosList* NewPos(Vec2 position, PosList* next)
{
    PosList* new = calloc(1, sizeof(PosList));
    new->position = position;
    new->next = next;
    return new;
}

// Add a position to the trail
void AddPos(Ball* ball, Vec2 position)
{
    ball->listSize++;

    PosList* new = NULL;
    PosList* first = ball->pastPositions;

    // Empty list?
    if (ball->pastPositions == NULL)
    {
        new = NewPos(position, first);
        ball->pastPositions = new;
        return;
    }

    new = NewPos(position, first);
    ball->pastPositions = new;

    // Max size exceeded? delete oldest node
    if (ball->listSize > MAX_SIZE)
    {
        PosList* current = new->next;
        PosList* prev;
        while (current->next != NULL)
        {
            prev = current;
            current = current->next;
        }
        free(current);
        prev->next = NULL;
    }
}

void Ball_Update(Game* game)
{
    Ball* ball = &game->ball;
    Pallet* palletA = &game->palletA;
    Pallet* palletB = &game->palletB;

    AddPos(ball, ball->position);

    // Change position based on speed and direction (velocity)
    Vec2 velocity = Vector2_Mul(ball->direction, ball->speed);
    velocity = Vector2_Mul(velocity, game->dt);
    ball->position = Vector2_Add(ball->position,
                                 velocity);

    // Handle collision
    WallColision(game);
    PalletCollision(game, palletA);
    PalletCollision(game, palletB);
}

// Recursively draws the trail
void DrawTrail(SDL_Renderer* renderer,
               const SDL_Color* color,
               PosList* current, int index)
{
    if (current == NULL)
        return;

    // color gets more and more transparent
    // as the trail goes
    SDL_Color newColor = *color;
    newColor.a -= Lerp(200.0f, 250.0f, index / (float)MAX_SIZE);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    DrawFillRect(renderer, &newColor,
                 current->position, BALL_SIZE, BALL_SIZE);

    DrawTrail(renderer, color, current->next, ++index);
}

void Ball_Draw(SDL_Renderer* renderer, Ball* ball)
{
    DrawFillRect(renderer, &white,
                 ball->position, BALL_SIZE, BALL_SIZE);
    DrawTrail(renderer, &white, ball->pastPositions, 0);
}