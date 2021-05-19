#include "game.h"
#include "Engine/input.h"
#include "Engine/vector.h"
#include "Engine/window.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int  FIELD_TOP;
int  FIELD_BOTTOM;
int  FIELD_RIGHT;
int  FIELD_LEFT;
Vec2 FIELD_MIDDLE;

static void InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

static void QuitSDL()
{
    TTF_Quit();
    SDL_Quit();
}

// Wrapper for TTF_OpenFont
static TTF_Font* LoadFont(const char* file, const int size)
{
    TTF_Font* font = TTF_OpenFont(file, size);
    if (!font) {
        fprintf(stderr, "Unable to open %s: %s\n", file, TTF_GetError());
        SDL_Quit();
    }
    return font;
}

// Loads all necessary resources into SDL
static void LoadResources(Game* game)
{
    game->numFonts = 2;
    game->font     = LoadFont("resources/font.ttf", 30);
}

// Resets the game parameters
// allowing replay
static void Reset(Game* game)
{
    game->totalSets  = 5;
    game->currentSet = 1;

    Pallet_Init(&game->palletA,
                Vector2(FIELD_RIGHT - 50 - PALLET_WIDTH, FIELD_MIDDLE.y - PALLET_HEIGHT / 2.0f),
                Vector2(-1.0f, 0.0f),
                SDL_SCANCODE_UP,
                SDL_SCANCODE_DOWN);

    Pallet_Init(&game->palletB,
                Vector2(FIELD_LEFT + 50, FIELD_MIDDLE.y - PALLET_HEIGHT / 2.0f),
                Vector2(1.0f, 0.0f),
                SDL_SCANCODE_A,
                SDL_SCANCODE_Z);

    game->ball.trail     = NULL;
    game->ball.trailLast = NULL;
    Ball_Reset(&game->ball, 1);

    game->state = GAME;
}

void Game_Init(Game* game)
{
    game->isRunning = SDL_TRUE;

    InitSDL();

    time_t t;
    srand((unsigned)time(&t));

    game->window = Window_Create("Engine", 720, 480, 0);

    FIELD_TOP    = 50;
    FIELD_BOTTOM = game->window->height - 50;
    FIELD_LEFT   = 20;
    FIELD_RIGHT  = game->window->width - 20;

    FIELD_MIDDLE.x = FIELD_LEFT + (FIELD_RIGHT - FIELD_LEFT) / 2.0f;
    FIELD_MIDDLE.y = FIELD_TOP + (FIELD_BOTTOM - FIELD_TOP) / 2.0f;

    Reset(game);

    LoadResources(game);
}

void Game_Quit(Game* game)
{
    Ball_Reset(&game->ball, 0);

    TTF_CloseFont(game->font);
    TTF_Quit();

    Window_Destroy(game->window);

    QuitSDL();
}

// Updates game struct with FPS info
// If targetFPS is >0 this will also limit it
static void CalculateFPS(Game* game, const unsigned int targetFPS)
{
    static Uint32 lastTime    = 0;
    Uint32        currentTime = SDL_GetTicks();

    Uint32 dt  = currentTime - lastTime;
    float  fps = 1.0f / (dt / 1000.0f);

    lastTime = currentTime;

    if (targetFPS > 0) {
        float targetTime = (1.0f / targetFPS) * 1000;

        if (dt < targetTime) {
            Uint32 delayTime = targetTime - dt;
            SDL_Delay(delayTime);

            dt += delayTime;
            fps = 1.0f / (dt / 1000.0f);
            lastTime += delayTime;
        }
    }
    game->dt = dt / 1000.0f;
    if (game->dt > 0.5f)
        game->dt = 0.016f;
    game->fps = fps;
}

static void Update(Game* game)
{
    game->isRunning = Input_Poll();

    if (IsKeyReleased(SDL_SCANCODE_ESCAPE))
        game->isRunning = SDL_FALSE;

    switch (game->state) {
        case GAME:
            Pallet_Update(&game->palletA, game->dt);
            Pallet_Update(&game->palletB, game->dt);
            Ball_Update(game);
            break;
        case END:
            if (IsKeyReleased(SDL_SCANCODE_Y))
                Reset(game);
            else if (IsKeyReleased(SDL_SCANCODE_N))
                game->isRunning = SDL_FALSE;
            break;
    }
}

void Game_Loop(Game* game)
{
    while (game->isRunning) {
        CalculateFPS(game, 60);
        Update(game);
        Render(game);
    }
}
