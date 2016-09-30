#include <SDL.h>
#include "initfile.h"

void init_window()
{
  /*mise en place de la fenetre principale*/
  SDL_Surface* affichage;
  
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
   
  SDL_WM_SetCaption("Labyrinthe", NULL);
  affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
  
}
