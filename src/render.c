#include "Engine/pallete.h"
#include "Engine/window.h"
#include "Engine/vector.h"

#include "game.h"

#include <SDL2/SDL_render.h>
#include <string.h>


// Set draw color
void Render_SetDrawColor(SDL_Renderer* renderer,
                         const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r,
                                     color->g,
                                     color->b,
                                     color->a);
}

void CopyTexture(SDL_Renderer* renderer,
                 SDL_Texture* texture,
                 Vec2 position, float width, float height)
{
    SDL_Rect rect = (SDL_Rect) {(int)position.x, (int)position.y,
                                (int)width, (int)height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

#define CENTERED -1

// Draw text on screen
void DrawText(const Game* game,
              const SDL_Color* color,
              Vec2 position,
              const char* text,
              TTF_Font* font)
{
    Window* window = game->window;
    SDL_Renderer* renderer = window->SDLRenderer;

    SDL_Surface* tmpSurface = TTF_RenderText_Blended(font, text, *color);
    float width = (float)tmpSurface->w;
    float height = (float)tmpSurface->h;

    SDL_Texture* label = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    if (position.x == CENTERED)
        position.x = (window->width - width)/2.0f;

    if (position.y == CENTERED)
        position.y = (window->height - height)/2.0f;

    CopyTexture(renderer, label, position, width, height);
}

void DrawPoint(SDL_Renderer* renderer, const SDL_Color* color,
               Vec2 p)
{
    Render_SetDrawColor(renderer, color);
    SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
}

void DrawLine(SDL_Renderer* renderer, const SDL_Color* color,
              Vec2 a, Vec2 b)
{
    Render_SetDrawColor(renderer, color);
    SDL_RenderDrawLine(renderer, (int)a.x, (int)a.y,
                                 (int)b.x, (int)b.y);
}

void DrawFillCircle(SDL_Renderer* renderer, const SDL_Color* color,
                    Vec2 center, float radius)
{
    for(int y=-radius; y<=radius; y++)
        for(int x=-radius; x<=radius; x++)
            if(x*x+y*y <= radius*radius)
                DrawPoint(renderer, color,
                          Vector2_Add(center, Vector2((float)x, (float)y)));
}

void DrawPlayField(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    Render_SetDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    char tmpString[30];

    sprintf(tmpString, "%d", game->palletA.score);
    DrawText(game, &white,
             Vector2(0.0f, 0.0f),
             tmpString,
             game->font);

    sprintf(tmpString, "%d", game->palletB.score);
    DrawText(game, &white,
             Vector2(game->window->width-30, 0.0f),
             tmpString,
             game->font);

    sprintf(tmpString, "%d/%d", game->currentSet, game->totalSets);
    DrawText(game, &white,
             Vector2(CENTERED, 0.0f),
             tmpString,
             game->font);

    Render_SetDrawColor(renderer, &grey);
    SDL_Rect playField;
    playField.x = game->fieldLeft;
    playField.y = game->fieldTop;
    playField.w = game->fieldRight - game->fieldLeft;
    playField.h = game->fieldBottom - game->fieldTop;
    SDL_RenderFillRect(renderer, &playField);
}

void Render(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    DrawPlayField(game);
    Pallet_Draw(renderer, &game->palletA);
    Pallet_Draw(renderer, &game->palletB);

    SDL_RenderPresent(renderer);
}
