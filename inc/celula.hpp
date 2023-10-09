#ifndef _CELULA_HPP_
#define _CELULA_HPP_

#include <SDL2/SDL.h> /* SDL_Point */

class Celula
{
    private:
        SDL_Point coords;
        bool      estado;

    public:
        Celula();
        int  x();
        int  y();
        void muere();
        void vive();
        bool e();
};

#endif
