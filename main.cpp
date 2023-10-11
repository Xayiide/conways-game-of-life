#include <iostream>  /* cerr              */
#include <cstdlib>   /* exit, rand, srand */
#include <time.h>    /* time              */
#include <stdint.h>  /* uint32_t          */
#include <chrono>    /* high res clock    */

#include <SDL2/SDL.h>

#define PXFACTOR   10
#define ANCHO      600
#define ALTO       600
#define PXANCHO    ANCHO / PXFACTOR
#define PXALTO     ALTO  / PXFACTOR
#define NUMCELULAS PXANCHO * PXALTO
#define GEN_SPEED  500 /* ms */

#define VIVA   true
#define MUERTA false

void limpia(bool celulas[PXALTO][PXANCHO])
{
    int f, c;

    for (f = 0; f < PXALTO; f++)
        for (c = 0; c < PXALTO; c++)
            celulas[f][c] = MUERTA;
}

bool evalua(bool celula, int vvivos)
{
    bool ret;

    if ((celula == VIVA) && (vvivos == 2 || vvivos == 3))
        ret = VIVA;
    else if ((celula == MUERTA) && (vvivos == 3))
        ret = VIVA;
    else
        ret = MUERTA;

    return ret;
}

bool noSeSale(int f, int c)
{
    bool ret = true;

    if ((f >= PXALTO) || (c >= PXANCHO))
        ret = false;

    return ret;
}

uint32_t vecinosVivos(bool celulas[PXALTO][PXANCHO], int f, int c)
{
    int i, fila, col;
    uint32_t vvivos = 0;

    int8_t despl[8][2] = {
        {-1, -1}, {-1,  0}, {-1,  1},
        { 0, -1},           { 0,  1},
        { 1, -1}, { 1,  0}, { 1,  1}
    };

    if (noSeSale(f, c) == false)
    {
        std::cerr << "Fuera de rango\n";
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 8; i++)
    {
        fila = f + despl[i][0];
        col  = c + despl[i][1];
        if (noSeSale(fila, col) == true)
            if (celulas[fila][col] == VIVA)
                vvivos++;
    }

    return vvivos;
}

void imprime(bool celulas[PXALTO][PXANCHO], SDL_Renderer *rnd, SDL_Point mouse)
{
    int f, c;

    SDL_SetRenderDrawColor(rnd, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rnd);

    SDL_SetRenderDrawColor(rnd, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (f = 0; f < PXALTO; f++)
    {
        for (c = 0; c < PXANCHO; c++)
            if (celulas[f][c] == true)
                SDL_RenderDrawPoint(rnd, f, c);
    }
    SDL_SetRenderDrawColor(rnd, 0, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(rnd, mouse.x, mouse.y);

}

void actualiza(bool celulas[PXALTO][PXANCHO])
{
    bool nueva[PXALTO][PXANCHO];
    bool celula;
    int  vvivos = 0;
    int  f, c;

    for (f = 0; f < PXALTO; f++)
    {
        for (c = 0; c < PXANCHO; c++)
        {
            celula = celulas[f][c];
            vvivos = vecinosVivos(celulas, f, c);
            celula = evalua(celula, vvivos);
            nueva[f][c] = celula;
        }
    }

    for (f = 0; f < PXALTO; f++)
        for (c = 0; c < PXANCHO; c++)
            celulas[f][c] = nueva[f][c];
}

void randCelulas(bool celulas[PXALTO][PXANCHO])
{
    int f, c;
    
    srand(time(NULL));

    for (f = 0; f < PXALTO; f++)
    {
        for (c = 0; c < PXANCHO; c++)
        {
            if (rand() % 2 == 0)
                celulas[f][c] = true;
            else
                celulas[f][c] = false;
        }
    }
}

int main()
{
    SDL_Event     e;
    SDL_Renderer *rnd = nullptr;
    SDL_Window   *win = nullptr;
    bool          run = true;
    bool          sim = true;
    bool          celulas[PXALTO][PXANCHO];
    SDL_Point     mouse;

    std::chrono::duration<double> max = std::chrono::milliseconds(GEN_SPEED);

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

    randCelulas(celulas);

    auto t1 = std::chrono::high_resolution_clock::now();
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
                    case SDLK_SPACE:
                        if (sim == true)
                            sim = false;
                        else
                            sim = true;
                        break;
                    case SDLK_1:
                        actualiza(celulas);
                        imprime(celulas, rnd, mouse);
                        break;
                    case SDLK_0:
                        limpia(celulas);
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                int auxx, auxy, x, y;
                /* Averiguar posición en la que estamos */
                SDL_GetMouseState(&auxx, &auxy);
                /* Nos devuelve el píxel en rango 0-ALTO, 0-ANCHO         */
                /* Hacemos módulo PXFACTOR para obtener el pixel adecuado */
                x = (auxx / (int) PXFACTOR);
                y = (auxy / (int) PXFACTOR);

                /* Colorearlo de azul */
                mouse.x = x;
                mouse.y = y;

            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int auxx, auxy, x, y;
                /* Averiguar el pixel en el que estamos */
                SDL_GetMouseState(&auxx, &auxy);
                x = (auxx / (int) PXFACTOR);
                y = (auxy / (int) PXFACTOR);

                if (celulas[x][y] == VIVA)
                    celulas[x][y] = MUERTA;
                else
                    celulas[x][y] = VIVA;
            }

        }
        auto t2 = std::chrono::high_resolution_clock::now();

        imprime(celulas, rnd, mouse);
        if ((t2 - t1) > max)
        {
            t1 = t2; /* Hay desviación local. TODO: Delay until? */
            if (sim == true)
                actualiza(celulas);
        }



        SDL_RenderPresent(rnd);
        SDL_Delay(15);

    }

    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
