#include <SDL.h>
#include "initfile.h"
#include "event.h"


void HandleEvent(SDL_Event event,int* game_over,int* mode)
{
  float a=0;
  float xplus;
  float yplus;
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
	    gameover();
	    SDL_Delay(2000);
            *game_over = 1;
            break;
	case SDLK_a:
            gameover();
	    *mode =0;
	  break;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
	  draw_screen();
	  *mode = 1;
	  SDL_WarpMouse(1024 / 2, 600 / 2);
	  break;
	case SDLK_q:
	case SDLK_LEFT:
	  xplus = -1;
	  break;
	case SDLK_d:
	case SDLK_RIGHT:
	  xplus = 1;
	  break;
	case SDLK_z:
	case SDLK_UP:
	  yplus = 1;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  yplus = -1;
	  break;
	  default:
	break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) 
      {
	case SDLK_q:
	case SDLK_LEFT:
	  xplus = 0;
	  break;
	case SDLK_d:
	case SDLK_RIGHT:
	  xplus = 0;
	  break;
	case SDLK_z:
	case SDLK_UP:
	  yplus = 0;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  yplus = 0;
	  break;
	  default:
	break;
      }
      break;
      case SDL_MOUSEMOTION:
      a += event.motion.xrel * .01;
      break;
      default:
	break;
  }
  deplacement(a, xplus, yplus);
}