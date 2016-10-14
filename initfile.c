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
    /*printf("******************************************\n****************************************\n******************GAME*******************\n******************OVER*******************\n****************************************\n******************************************\n");
*/
  
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

void draw_screen()
{
  printf("test\n");
  SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 0, 0, 255));
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  /*fonction exemple*/
  const int colors[] = {127, 127, 127};
  int ncolors = sizeof(colors)/(sizeof(int)*3);

  int w = affichage->w/2;
  for (int i=0; i<mapw; i++) { // draw the map
        for (int j=0; j<maph; j++) {
            if (map[i+j*mapw]==' ') continue;
            SDL_Rect tmp;
            tmp.w = 16;
            tmp.h = 16;
            tmp.x = i*16 + w;
            tmp.y = j*16;
            int z = ((i+j*mapw)%ncolors)*3;
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(sdl_screen_->format, colors[z], colors[z+1],colors[z+2]));
        }
    }


}
