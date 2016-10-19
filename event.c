#include <SDL.h>
#include "initfile.h"
#include "event.h"


void HandleEvent(SDL_Event event,int* game_over,int* mode, SDL_Rect depl)
{
  float a=0;
  

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
	  *mode ++;
	  SDL_WarpMouse(1024 / 2, 600 / 2);
	  break;
	case SDLK_q:
	case SDLK_LEFT:
	  depl.x--;
          break;
	case SDLK_d:
	case SDLK_RIGHT:
	  depl.x++;
          break;
	case SDLK_z:
	case SDLK_UP:
	  depl.y++;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  depl.y--;
	  break;
	  default:
	break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) 
      /*{
	case SDLK_q:
	case SDLK_LEFT:
	  depl.x = 0;
	  break;
	case SDLK_d:
	case SDLK_RIGHT:
	  depl.x = 0;
	  break;
	case SDLK_z:
	case SDLK_UP:
	  depl.y = 0;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  depl.y = 0;
	  break;
	  default:
	break;
      }
      break;*/
      case SDL_MOUSEMOTION:
      a += event.motion.xrel * .01;
      break;
      default:
	break;
  }
  deplacement(a, depl);
}