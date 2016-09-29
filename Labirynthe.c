#include <SDL.h>
#include "game.h"

/*mise en place de la fenetre principale*/
SDL_Surface* affichage;




int main()
{
<<<<<<< HEAD
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  
  
  SDL_WM_SetCaption("LABY", NULL);
  affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    
    

    
=======
  init_window();    
>>>>>>> 929ad76a9138d3d8d6b7ac75877e3d0df7ff8d02
  int a;
  printf("damien est u  n vrai beau goss!\n");
  scanf("%d",&a);
  test(a);
  atexit(SDL_Quit);	
  return EXIT_SUCCESS;   

}
