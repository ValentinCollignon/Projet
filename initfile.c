#include <SDL.h>
#include "initfile.h"

/*mise en place de la fenetre principale*/
SDL_Surface * affichage;

void init_window()
{

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
   
  SDL_WM_SetCaption("Labyrinthe", NULL);
  affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
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
}
/*
int wait()
{
 int i,j;
 
 for(i=0; i<10000; i++)
 {
     for(j=0; j<10000; j++)
     {}
     
}
 return 0;
}
*/
void gameover()
{
    SDL_Surface *temp, *gamover;
    SDL_Rect rcgameover;
    int colorkey;
    colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
    rcgameover.x = 0;
    rcgameover.y = 0;
    temp  = SDL_LoadBMP("image/game_over.bmp");
    gamover = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_SetColorKey(gamover, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    SDL_BlitSurface(gamover, NULL, affichage, &rcgameover);
    SDL_UpdateRect(affichage, 0, 0, 0, 0);
    printf("******************************************\n****************************************\n******************GAME*******************\n******************OVER*******************\n****************************************\n******************************************\n");
} 
/*
void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel)
{
  int bpp = theScreen->format->BytesPerPixel;
  Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x*bpp;
  for (int i=0; i<bpp; i++) 
  {
    p[i] = ((Uint8*)&pixel)[i];
    
  }
}
 
*/
