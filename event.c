#include <SDL.h>
#include "initfile.h"
#include "event.h"

float a=0;
void HandleEvent(SDL_Event event, int* game_over, int* mode, SDL_Rect depl)

{
  
  /*SDL_Rect depl;*/

  switch (event.type) /*keystate = SDL_GetKeyState(NULL)*/
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
	  depl.x -= 1;
          break;
	case SDLK_d:
	case SDLK_RIGHT:
	  depl.x += 1;
          break;
	case SDLK_z:
	case SDLK_UP:
	  depl.y += 1;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  depl.y -= 1;
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
      break;
      case SDL_MOUSEMOTION:
      a += event.motion.xrel * .01;
      break;
      default:
	break;
  }

  if (*mode ==1)
  {
    deplacement(a, depl);
  }

}