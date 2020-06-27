#ifndef GAME_H
#define GAME_H

#include "pallet.h"
#include "ball.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Window;
struct Input;

enum GameState {
    GAME,
    END
};

// Contains data necessary to run the game
typedef struct Game {
    struct Window*  window;

    // Game internal information
    SDL_bool        isRunning;
    float           fps;
    float           dt;
    enum GameState  state;

    // Subsystems
    struct Input*   input;

    // Font files
    TTF_Font*       font;
    int             numFonts;

    // Boundaries of the playing field
    int             fieldTop;
    int             fieldBottom;
    int             fieldRight;
    int             fieldLeft;
    Vec2            fieldMiddle; // Middle point of the playing field

    // Sets
    int             totalSets;
    int             currentSet;

    // Actors
    Pallet          palletA;    // Right Pallet
    Pallet          palletB;    // Left Pallet
    Ball            ball;
} Game;


// Sets up SDL and creates a window and a renderer, filling up
void Game_Init(Game* game);

// Deallocates memory and quits SDL
void Game_Quit(Game* game);

// Game loop...
void Game_Loop(Game* game);

#endif
