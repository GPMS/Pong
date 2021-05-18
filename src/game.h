#pragma once

#include "ball.h"
#include "pallet.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Window;
struct Input;

enum GameState {
    GAME,
    END
};

extern int  FIELD_TOP;
extern int  FIELD_BOTTOM;
extern int  FIELD_RIGHT;
extern int  FIELD_LEFT;
extern Vec2 FIELD_MIDDLE;

// Contains data necessary to run the game
typedef struct Game {
    struct Window* window;

    // Game internal information
    SDL_bool       isRunning;
    float          fps;
    float          dt;
    enum GameState state;

    // Font files
    TTF_Font* font;
    int       numFonts;

    // Sets
    int totalSets;
    int currentSet;

    // Actors
    Pallet palletA;  // Right Pallet
    Pallet palletB;  // Left Pallet
    Ball   ball;
} Game;

// Sets up SDL and creates a window and a renderer, filling up
void Game_Init(Game* game);

// Deallocates memory and quits SDL
void Game_Quit(Game* game);

// Game loop...
void Game_Loop(Game* game);
