#include <SDL.h>
#include "initfile.h"
#include <math.h>


#define mapc 32
#define mapl 32


const int colors[] = {120, 120, 220};
const float fov = M_PI/3;



/*mise en place de la fenetre principale*/
SDL_Surface * affichage;


/* map*/
char map[mapl*mapc+1] = "\
################################\
#                            # #\
# ######### ###### ##### ##### #\
# #       #      #     # #     #\
# # ##### ###### ##### # ##### #\
# ###   #            # #     # #\
#     ############## # ####### #\
#### ##                      # #\
#               ##           # #\
#                              #\
#                            # #\
#                            # #\
#                            # #\
#          # ################# #\
#          #                 # #\
#          # ### #### ###### # #\
#          # #        #    # # #\
#          #        # # #### # #\
#          # #### ### #      # #\
#                     ###### # #\
##################### ##   # # #\
#    #   #   #        #  #   # #\
# ##   #   #    # ############ #\
#################     #   #  # #\
#               ##### # # # ## #\
# ########### # #     # # #  # #\
# #         # ### ### # #### # #\
# # ######### #   #          # #\
# #           # # #  ######### #\
# ############# # #  #   #   # #\
#               # #    #   # # #\
################################";


void init_window()
{

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
   
  SDL_WM_SetCaption("Labyrinthe", NULL);
  affichage = SDL_SetVideoMode(1024, 600, 32, SDL_SWSURFACE);
  init_menu();
}


void init_menu()
{
  SDL_Surface *temp, *menu;
  SDL_Rect rcmenu;
  int colorkey;
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rcmenu.x = 0;
  rcmenu.y = 0;
  temp  = SDL_LoadBMP("image/menu.bmp");
  menu = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_SetColorKey(menu, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  SDL_BlitSurface(menu, NULL, affichage, &rcmenu);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  printf("fonction init_menu\n");
}

void gameover()
{
    SDL_Surface *temp, *gamover;
    SDL_Rect rcgameover;
    int colorkey;
    colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
    rcgameover.x = 0;
    rcgameover.y = 0;

    temp  = SDL_LoadBMP("image/game_over2.bmp");

    gamover = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_FreeSurface(affichage);
    SDL_SetColorKey(gamover, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    
    SDL_BlitSurface(gamover, NULL, affichage, &rcgameover);

    SDL_UpdateRect(affichage, 0, 0, 0, 0);
    printf("fonction gameover\n");

  
} 

void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel) 
{
    int bpp = theScreen->format->BytesPerPixel;
    Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x*bpp;
    int i;
    for (i=0; i<bpp; i++) 
    {
        p[i] = ((Uint8*)&pixel)[i];
    }
}

void draw_screen()
{

  SDL_Rect tmp;
    int ncolors, i, j, w, z;
    printf("fonction draw_screen\n");
    SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
    SDL_UpdateRect(affichage, 0, 0, 0, 0);
    /*draw map*/
    ncolors = sizeof(colors)/(sizeof(int)*3);
    w = affichage->w/2;
    tmp.w = 16;
    tmp.h = 16;
    for (i=0; i<mapl; i++) 
    { 
        for (j=0; j<mapc; j++) 
        {
            if (map[i+j*mapl]==' ') continue;
            tmp.x = i*16 + w;
            tmp.y = j*16;
            z = ((i+j*mapl)%ncolors)*3;
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, colors[z], colors[z+1],colors[z+2]));
            SDL_UpdateRect(affichage, 0, 0, 0, 0);
            
        }
    }
   /* regarde ca!!!!
    for (i=0; i<w; i++) 
    {
      float t;
      float ca = (1.-i/float(w)) * (a-fov/2.) + i/float(w)*(a+fov/2.);
      for (t=0; t<20; t+=.05)
      {
	float cx = cos(ca)*t;
        float cy = sin(ca)*t;
        putpixel(affichage, w+cx*16, cy*16, 0); 
        int idx = int(cx)+int(cy)*mapw;
	if (map[idx]!=' ') 
	{
	  int h = affichage->h/t;
          SDL_Rect tmp;
          tmp.w = 1;
          tmp.h = h;
          tmp.x = i;
          tmp.y = (affichage->h-h)/2;
          int z = (idx%ncolors)*3;
          SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, colors[z], colors[z+1],colors[z+2]));
          break;
	  
	}
        }
    }
    SDL_Flip(affichage);
*/

}

void end()
{
  SDL_FreeSurface(affichage);
  SDL_Quit();
}
