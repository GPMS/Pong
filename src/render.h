#ifndef RENDER_H
#define RENDER_H

struct Game;


// Changes the draw color
void Render_SetDrawColor(SDL_Renderer* renderer,
                         const SDL_Color* color);

// Draws the game
void Render(struct Game* game);

#endif

