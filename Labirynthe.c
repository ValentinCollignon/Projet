#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

/*mise en place de la fenetre principale*/
SDL_Surface* affichage;




int main()
{
  init_window()
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  
  
    SDL_WM_SetCaption("LABY", NULL);
    affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    
    
    atexit(SDL_Quit);
    return EXIT_SUCCESS;
    
    
}
