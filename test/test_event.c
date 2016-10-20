#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>


int main ()
{
    SDL_Surface *ecran = NULL, *rectangle = NULL;
 
    int continuer = 1;// tmp = 0;
    Uint8 *touche = NULL;
 
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
 
    ecran = SDL_SetVideoMode(800, 640, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
 
    SDL_Rect pos_r;
      pos_r.x = 350;
      pos_r.y = 300;
 
    while(continuer)
    {
        touche = SDL_GetKeyState(NULL);
 
        SDL_PollEvent(&event);
 
        if(touche[SDLK_ESCAPE])
            continuer = 0;
 
        if(touche[SDLK_RIGHT])
            pos_r.x ++;
 
        if(touche[SDLK_LEFT])
            pos_r.x --;
 
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 0, 255));
        SDL_BlitSurface(rectangle, NULL, ecran, &pos_r);
 
        SDL_Flip(ecran);
    }
    SDL_free(rectangle);
    SDL_free(ecran);
 
    return EXIT_SUCCESS;
}