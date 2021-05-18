#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#include "Engine/vector.h"

struct Game;

// Changes the draw color
void Render_SetDrawColor(SDL_Renderer* renderer, const SDL_Color* color);

// Draws a rectangle filled with the given color
void DrawFillRect(SDL_Renderer* renderer, const SDL_Color* color, Vec2 position, int width, int height);

// Draws the game
void Render(struct Game* game);

#endif
