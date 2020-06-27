#include "game.h"
#include "render.h"
#include "Engine/pallete.h"
#include <stdlib.h>

const int BALL_SIZE = 10;
const float BALL_INITIAL_SPEED = 100.0f;
const float BALL_MAX_SPEED = 600.0f;
const int MAX_SIZE = 50;


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

void PalletCollision(Game* game, Pallet* pallet)
{
    Ball* ball = &game->ball;

    Vec2 ballTopLeft = ball->position;
    Vec2 ballTopRight = Vector2_Add(ballTopLeft, Vector2(BALL_SIZE, 0.0f));
    Vec2 ballBottomLeft = Vector2_Add(ballTopLeft, Vector2(0.0f, BALL_SIZE));
    Vec2 ballBottomRight = Vector2_Add(ballTopLeft, Vector2(BALL_SIZE, BALL_SIZE));

    if (Vector2_Dot(ball->direction, pallet->orientation) > 0)
        return;

    if (PointRectCollision(ballTopLeft, pallet) ||
        PointRectCollision(ballTopRight, pallet) ||
        PointRectCollision(ballBottomLeft, pallet) ||
        PointRectCollision(ballBottomRight, pallet))
    {
        if (ball->position.x < game->fieldLeft + 100)
            ball->position.x = pallet->position.x + PALLET_WIDTH + 1;
        else
            ball->position.x = pallet->position.x - BALL_SIZE - 1;

        ball->direction.x *= -1.0f;

        if (ball->position.y > pallet->position.y+(PALLET_HEIGHT/2.0f))
            ball->direction.y += 0.5f;
        else
            ball->direction.y -= 0.5f;

        ball->speed *= 1.1f;
        if (ball->speed > BALL_MAX_SPEED)
            ball->speed = BALL_MAX_SPEED;
    }
}

void Ball_Reset(Game* game, int turn)
{
    Ball* ball = &game->ball;

    if (game->currentSet > game->totalSets)
        game->state = END;

    ball->speed = BALL_INITIAL_SPEED;
    ball->position.y = game->fieldTop +
                       (game->fieldBottom - game->fieldTop)/2.0f;
    ball->position.x = game->fieldLeft +
                       (game->fieldRight - game->fieldLeft)/2.0f;

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

    if (turn == 0)
        ball->direction = Vector2(1.0f, 0.0f);
    else
        ball->direction = Vector2(-1.0f, 0.0f);

    int randomAngle = rand() % 60;
    int isDown = rand() % 2;
    randomAngle = isDown ? randomAngle : -randomAngle;

    ball->direction = Vector2_Rotate(ball->direction,
                                     DegreesToRadians(randomAngle));
}

void WallColision(Game* game)
{
    Ball* ball = &game->ball;
    Pallet* palletA = &game->palletA;
    Pallet* palletB = &game->palletB;

    if (ball->position.y < game->fieldTop)
    {
        ball->position.y = game->fieldTop;
        ball->direction.y *= -1.0f;
    }
    if (ball->position.y + BALL_SIZE > game->fieldBottom)
    {
        ball->position.y = game->fieldBottom - BALL_SIZE;
        ball->direction.y *= -1.0f;
    }

    if (ball->position.x + BALL_SIZE > game->fieldRight)
    {
        palletB->score++;
        game->currentSet++;
        Ball_Reset(game, game->currentSet % 2);
    }
    if (ball->position.x < game->fieldLeft)
    {
        palletA->score++;
        game->currentSet++;
        Ball_Reset(game, game->currentSet % 2);
    }
}

void AddPos(Ball* ball, Vec2 position)
{
    ball->listSize++;

    if (ball->pastPositions == NULL)
    {
        PosList* new = calloc(1, sizeof(PosList));
        new->position = position;
        ball->pastPositions = new;
        return;
    }

    PosList* first = ball->pastPositions;

    PosList* new = calloc(1, sizeof(PosList));
    new->position = position;
    new->next = first;
    ball->pastPositions = new;

    if (ball->listSize > MAX_SIZE)
    {
        PosList* current = ball->pastPositions;
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

    Vec2 velocity = Vector2_Mul(ball->direction, ball->speed);
    velocity = Vector2_Mul(velocity, game->dt);

    AddPos(ball, ball->position);
    ball->position = Vector2_Add(ball->position,
                                 velocity);

    WallColision(game);
    PalletCollision(game, palletA);
    PalletCollision(game, palletB);
}

void DrawTrail(SDL_Renderer* renderer,
               const SDL_Color* color,
               PosList* current, int i)
{
    if (current == NULL)
        return;

    SDL_Color newColor = *color;
    float percent = i / (float)MAX_SIZE;
    newColor.a -= Lerp(200.0f, 250.0f, percent);

    SDL_Rect rect;
    rect.x = (int)current->position.x;
    rect.y = (int)current->position.y;
    rect.w = rect.h = BALL_SIZE;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    Render_SetDrawColor(renderer, &newColor);
    SDL_RenderFillRect(renderer, &rect);

    DrawTrail(renderer, color, current->next, ++i);
}

void Ball_Draw(SDL_Renderer* renderer, Ball* ball)
{
    SDL_Rect rect;
    rect.x = (int)ball->position.x;
    rect.y = (int)ball->position.y;
    rect.w = rect.h = BALL_SIZE;

    Render_SetDrawColor(renderer, &white);
    SDL_RenderFillRect(renderer, &rect);

    DrawTrail(renderer, &white, ball->pastPositions, 0);
}