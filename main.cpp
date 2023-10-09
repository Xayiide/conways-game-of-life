#include <iostream>  /* cerr                  */
#include <cstdlib>   /* exit, rand, srand     */
#include <time.h>    /* time                  */
#include <vector>    /* std::vector           */

#include <SDL2/SDL.h>

#include "inc/celula.hpp"

#define PXFACTOR   100
#define ANCHO      600
#define ALTO       600
#define PXANCHO    ANCHO / PXFACTOR
#define PXALTO     ALTO  / PXFACTOR
#define NUMCELULAS PXANCHO * PXALTO

void randCelulas(std::vector<Celula>& celulas)
{
    int f, c;
    
    srand(time(NULL));

    for (f = 0; f < PXALTO; f++)
    {
        for (c = 0; c < PXANCHO; c++)
        {
            if (rand() % 1 == 0)
                celulas[f * ANCHO + c].vive();
            else
                celulas[f * ANCHO + c].muere();
        }
    }
}

int numVecinos(std::vector<Celula> celulas, Celula c)
{
    int num = 0;

    return num;
}


int main()
{
    SDL_Event     e;
    SDL_Renderer *rnd = nullptr;
    SDL_Window   *win = nullptr;
    bool          run = true;

    std::vector<Celula> celulas(NUMCELULAS);
    randCelulas(celulas);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        std::cerr << "Error en la inicialización de SDL.\n";
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow("Juego de la vida de Conway",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           ANCHO, ALTO, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        std::cerr << "Error en la creación de la ventana.\n";
        exit(EXIT_FAILURE);
    }

    rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL)
    {
        std::cerr << "Error en la creación del render.\n";
        exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(rnd, PXANCHO, PXALTO);

    while (run)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_q:
                        run = false;
                        break;
                }
            }

        }
    
        SDL_SetRenderDrawColor(rnd, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rnd);

        SDL_SetRenderDrawColor(rnd, 255, 255, 255, SDL_ALPHA_OPAQUE);
        for (int f = 0; f < PXALTO; f++)
            for (int c = 0; c < PXANCHO; c++)
                if (celulas[f * ANCHO + c].e() == true)
                    SDL_RenderDrawPoint(rnd, celulas[f * ANCHO + c].x(),
                                             celulas[f * ANCHO + c].y());

        SDL_RenderPresent(rnd);

    }

    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
