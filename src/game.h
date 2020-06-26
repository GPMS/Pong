#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Window;
struct Input;
struct Timer;

enum GameState {
    GAME
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
    struct Timer*   timerList;

    // Font files
    TTF_Font*       fonts[2];
    int             numFonts;
} Game;

// Sets up SDL and creates a window and a renderer, filling up
void Game_Init(Game* game);

// Deallocates memory and quits SDL
void Game_Quit(Game* game);

// Game loop...
void Game_Loop(Game* game);

#endif
