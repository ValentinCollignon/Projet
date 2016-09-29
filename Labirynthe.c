#include <SDL.h>
#include "game.h"

/*mise en place de la fenetre principale*/
SDL_Surface* affichage;




int main()
{
<<<<<<< HEAD
  init_window()
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  
  
    SDL_WM_SetCaption("LABY", NULL);
    affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    
    
    atexit(SDL_Quit);
    return EXIT_SUCCESS;
    
    
=======
    int a;
    printf("damien est u  n vrai beau goss!\n");
    scanf("%d",&a);
    test(a);
    return EXIT_SUCCESS;   
>>>>>>> 403511e6a54439439d217ebc9b4e028cf8b79ac9
}
