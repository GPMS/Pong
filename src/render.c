#include "Engine/pallete.h"
#include "Engine/vector.h"
#include "Engine/window.h"

#include "game.h"

#include <string.h>

// Set draw color
void Render_SetDrawColor(SDL_Renderer* renderer, const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

void CopyTexture(SDL_Renderer* renderer, SDL_Texture* texture, Vec2 position, float width, float height)
{
    SDL_Rect rect = (SDL_Rect){(int)position.x, (int)position.y, (int)width, (int)height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

#define CENTERED -1

// Draw text on screen
void DrawText(const Game* game, const SDL_Color* color, Vec2 position, const char* text, TTF_Font* font)
{
    Window*       window   = game->window;
    SDL_Renderer* renderer = window->SDLRenderer;

    SDL_Surface* tmpSurface = TTF_RenderText_Blended(font, text, *color);
    float        width      = (float)tmpSurface->w;
    float        height     = (float)tmpSurface->h;

    SDL_Texture* label = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    if (position.x == CENTERED)
        position.x = (window->width - width) / 2.0f;

    if (position.y == CENTERED)
        position.y = (window->height - height) / 2.0f;

    CopyTexture(renderer, label, position, width, height);
    SDL_DestroyTexture(label);
}

void DrawPoint(SDL_Renderer* renderer, const SDL_Color* color, Vec2 p)
{
    Render_SetDrawColor(renderer, color);
    SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
}

void DrawLine(SDL_Renderer* renderer, const SDL_Color* color, Vec2 a, Vec2 b)
{
    Render_SetDrawColor(renderer, color);
    SDL_RenderDrawLine(renderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void DrawFillCircle(SDL_Renderer* renderer, const SDL_Color* color, Vec2 center, float radius)
{
    for (int y = -radius; y <= radius; y++)
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
                DrawPoint(renderer, color, Vector2_Add(center, Vector2((float)x, (float)y)));
}

void DrawFillRect(SDL_Renderer* renderer, const SDL_Color* color, Vec2 position, int width, int height)
{
    SDL_Rect rect;
    rect.x = (int)position.x;
    rect.y = (int)position.y;
    rect.w = width;
    rect.h = height;

    Render_SetDrawColor(renderer, color);
    SDL_RenderFillRect(renderer, &rect);
}

static void DrawPlayField(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    Render_SetDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    char tmpString[30];

    sprintf(tmpString, "%d", game->palletB.score);
    DrawText(game, &white, Vector2(FIELD_LEFT, 0.0f), tmpString, game->font);

    sprintf(tmpString, "%d", game->palletA.score);
    DrawText(game, &white, Vector2(FIELD_RIGHT - 40, 0.0f), tmpString, game->font);

    sprintf(tmpString, "%d/%d", game->currentSet, game->totalSets);
    DrawText(game, &white, Vector2(CENTERED, 0.0f), tmpString, game->font);

    Render_SetDrawColor(renderer, &grey);
    SDL_Rect playField;
    playField.x = FIELD_LEFT;
    playField.y = FIELD_TOP;
    playField.w = FIELD_RIGHT - FIELD_LEFT;
    playField.h = FIELD_BOTTOM - FIELD_TOP;
    SDL_RenderFillRect(renderer, &playField);
}

static void DrawEnd(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;
    Pallet*       a        = &game->palletA;
    Pallet*       b        = &game->palletB;

    Render_SetDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    char tmpString[30];

    int center = game->window->height / 2.0f;

    sprintf(tmpString, "Player %c won!", a->score > b->score ? 'A' : 'B');
    DrawText(game, &white, Vector2(CENTERED, center - 40), tmpString, game->font);
    sprintf(tmpString, "Will you play again?");
    DrawText(game, &white, Vector2(CENTERED, center), tmpString, game->font);
    sprintf(tmpString, "(Y/N)");
    DrawText(game, &white, Vector2(CENTERED, center + 40), tmpString, game->font);
}

void Render(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    switch (game->state) {
        case GAME:
            DrawPlayField(game);
            Pallet_Draw(renderer, &game->palletA);
            Pallet_Draw(renderer, &game->palletB);
            Ball_Draw(renderer, &game->ball);
            break;
        case END:
            DrawEnd(game);
    }

    SDL_RenderPresent(renderer);
}
