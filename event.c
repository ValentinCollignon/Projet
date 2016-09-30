#include <SDL.h>
#include "event.h"


void HandleEvent(SDL_Event event,int* game_over)
{
  switch (event.type) 
  {
    /* close button clicked */
    case SDL_QUIT:
      *game_over = 1;
      break;
    /* handle the keyboard */
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) 
      {
	case SDLK_ESCAPE:
	case SDLK_q:
	  *game_over=1;
	  break;
	
      }
  }
}