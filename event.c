#include <SDL.h>
#include "initfile.h"
#include "event.h"

float a=0;
int pause = 0;
int posx=0, posy=0;
int posSx,posSy;

void HandleEvent(SDL_Event event,int* game_over,int* mode)
{
  
  SDL_Rect depl;
  /*keystate = SDL_GetKeyState(NULL);*/


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
            *game_over = 1;
            break;
            
	case SDLK_a:
	  if (*mode == 1)
	  {
	    pause = 1;
	    *mode = 0;
	    
	  }
	  
	  break;
          
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
	  if ((*mode == 0) && (pause == 0))
	  {
	    printf("diff \n");
	    choixdiffi();
	    *mode = 2;
	  }
	  if (pause == 1)
	  {
	    *mode = 1;
	  }
	  if ((*mode == 3) )
	  {
	    *mode = 1;
	    creamap();
	    draw_screen();
	  }
	  
	  break;
	  
	case SDLK_q:
	case SDLK_LEFT:
	  depl.x = -1;
          break;
	  
	case SDLK_d:
	case SDLK_RIGHT:
	  depl.x = 1;
	  break;
	  
	case SDLK_z:
	case SDLK_UP:
	  depl.y = 1;
	  break;
	  
	case SDLK_s:
	case SDLK_DOWN:
	  depl.y = -1;
	  break;
	  
	default:
	break;
	case SDLK_m:
	    draw_minicarte();
	    *mode =0;
	    break;
	    
	case SDLK_o :
	  full();
	  break;
	
	
      }
      break;
      case SDL_MOUSEMOTION:
	
      a += event.motion.xrel * .01;
      posSx = event.motion.x ;
      posSy = event.motion.y ;

      break;
      
      case SDL_MOUSEBUTTONDOWN:
	  case SDL_BUTTON_LEFT:
	  if (*mode ==2)
	  {
	    
	    if( ((posSx >= 128 && posSx <= 256 && posSy >= 128 && posSy <= 256)))
	      {
		difficulte(0);
		 printf("facile\n");
		*mode =1;
		creamap();
		draw_screen();
	      }
	      if( ((posSx >= AFFICHAGE_WIDTH/2 + 128 && posSx <= AFFICHAGE_WIDTH/2 +256 && posSy >= 128 && posSy <= 256)))
	      {
		difficulte(1);
		printf("moyen\n");
		*mode =1;
		creamap();
		draw_screen();
	      }
	      if( ((posSx >= 128 && posSx <= 256 && posSy >= AFFICHAGE_HEIGHT/2 + 128 && posSy <= AFFICHAGE_HEIGHT/2 + 256)))
	      {
		difficulte(2);
		printf("diffi\n");
		*mode =1;
		creamap();
		draw_screen();
	      }
	      if( ((posSx >= AFFICHAGE_WIDTH/2 + 128 && posSx <= AFFICHAGE_WIDTH/2 + 256 && posSy >= AFFICHAGE_HEIGHT/2 + 128 && posSy <= AFFICHAGE_HEIGHT/2 + 256)))
	      {
		difficulte(3);
		printf("alea\n");
		*mode =1;
		creamap();
		draw_screen();
	      }
	      
	  }
	  break;
      break;
    
      
      case SDL_KEYUP:
	switch (event.key.keysym.sym) 
	{
	  case SDLK_m:
	    *mode =1;
	    break;
	  default:
	    break;
	}
	break;
      default:
	break;

  }

  if (*mode == 1)
  {
    deplacement(a, depl,mode);
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor (SDL_DISABLE);

  }
  else
  {
    SDL_ShowCursor (SDL_ENABLE);
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  }

}