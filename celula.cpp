#include "inc/celula.hpp"

#include <SDL2/SDL.h>

Celula::Celula()
{
    coords.x = 0;
    coords.y = 0;
    estado   = false;
}

void Celula::muere()
{
    estado = false;
}

void Celula::vive()
{
    estado = true;
}

bool Celula::e()
{
    return estado;
}

int Celula::x()
{
    return coords.x;
}

int Celula::y()
{
    return coords.y;
}
