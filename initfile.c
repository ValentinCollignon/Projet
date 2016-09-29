#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "game.h"

void init_window()
{
  /*mise en place de la fenetre principale*/
  SDL_Surface* affichage;
  
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
   
  SDL_WM_SetCaption("LABYT A BIBI", NULL);
  affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
  
}
