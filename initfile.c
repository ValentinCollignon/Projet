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
  
}

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

void gameover()
{
    SDL_Surface *temp, *gamover;
    SDL_Rect rcgameover;
    int colorkey;
    colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
    rcgameover.x = 0;
    rcgameover.y = 0;
    
    temp  = SDL_LoadBMP("Bliss.bmp");
    gamover = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_SetColorKey(gamover, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    
    SDL_BlitSurface(gamover, NULL, affichage, &rcgameover);
    printf("******************************************\n****************************************\n******************GAME*******************\n******************OVER*******************\n****************************************\n******************************************\n");
    
} 