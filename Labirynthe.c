#include <SDL.h>
#include "game.h"


int main()
{
  init_window();    

  int a;
  printf("damien est u  n vrai beau goss!\n");
  scanf("%d",&a);
  test(a);
  atexit(SDL_Quit);	
  return EXIT_SUCCESS;   

}
