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

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}