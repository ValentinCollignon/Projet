#include <SDL.h>
#include "game.h"



int main()
{
  int game_over = 0;
  init_window();    
  while(game_over != 1)
  {
    SDL_Event event;
    
    /* look for an event */
    if(SDL_PollEvent(&event))
    {
      HandleEvent(event);
      
    }
    printf("pour quit : 1 puis entrée!\n");
    scanf("%d",&game_over);
    test(game_over);
  }
  end();
  return EXIT_SUCCESS;   

}
