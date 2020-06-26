#include "game.h"
#include "Engine/window.h"
#include "Engine/input.h"
#include "Engine/timer.h"
#include "Engine/vector.h"
#include "render.h"
#include <stdio.h>


void InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

void QuitSDL()
{
    TTF_Quit();
    SDL_Quit();
}

// Wrapper for TTF_OpenFont
TTF_Font* LoadFont(const char* file,
                   const int size)
{
    TTF_Font* font = TTF_OpenFont(file, size);
    if (!font)
    {
        fprintf(stderr, "Unable to open %s: %s\n", file, TTF_GetError());
        SDL_Quit();
    }
    return font;
}

// Loads all necessary resources into SDL
void LoadResources(Game* game)
{
    game->numFonts = 2;
    game->fonts[0] = LoadFont("resources/font.ttf", 15);
    game->fonts[1] = LoadFont("resources/font.ttf", 30);
}

void Game_Init(Game* game)
{
    InitSDL();

    game->window = Window_Create("Engine", 720, 480, 0);

    game->input = Input_Create();

    game->state = GAME;
    game->isRunning = SDL_TRUE;

    LoadResources(game);
}

void Game_Quit(Game* game)
{
    Input_Destroy(game->input);

    Timer_EmptyList(game->timerList);

    for (int i = 0; i < game->numFonts; i++)
        TTF_CloseFont(game->fonts[i]);
    TTF_Quit();

    Window_Destroy(game->window);

    QuitSDL();
}

// Updates game struct with FPS info
// If targetFPS is >0 this will also limit it
void CalculateFPS(Game* game, const unsigned int targetFPS)
{
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    Uint32 dt = currentTime - lastTime;
    float fps = 1.0f / (dt / 1000.0f);

    lastTime = currentTime;

    if (targetFPS > 0)
    {
        float targetTime = (1.0f / targetFPS)*1000;

        if (dt < targetTime)
        {
            Uint32 delayTime = targetTime - dt;
            SDL_Delay(delayTime);

            dt += delayTime;
            fps = 1.0f / (dt / 1000.0f);
            lastTime += delayTime;
        }

    }
    game->dt = dt / 1000.0f;
    game->fps = fps;

}

void CallF(Game* game)
{
    printf("timer finished\n");
}

// Deals with SDL events
void ProcessInput(Game* game)
{
    Input* input = game->input;

    game->isRunning = Input_Get(input);

    if (Input_KeyWasReleased(input, SDLK_ESCAPE))
        game->isRunning = SDL_FALSE;

    if (Input_ButtonIsDoubleClicked(input, SDL_BUTTON_LEFT))
        game->timerList = Timer_Add(game->timerList, 5 * 1000, SDL_TRUE, CallF);
}

void Update(Game* game)
{

}

void Game_Loop(Game* game)
{
    while (game->isRunning)
    {
        CalculateFPS(game, 60);
        printf("\ndt: %f", game->dt);

        Timer_Tick(game);

        ProcessInput(game);
        Update(game);
        Render(game);

    }
}
