#include <SDL.h>
#include "initfile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define mapc 32
#define mapl 32
#define SIZE mapc*mapl+1

const int colors[] = {120, 120, 220};
const float fov = M_PI/3;
float x = 16.3;
float y = 16.2;

/*SDL_Rect position;
position.x = 16;
position.y = 16;*/
float a2=0;
/*mise en place de la fenetre principale*/
SDL_Surface * affichage;
char* map;


char* lireMap(char* nomFichier)
{
  char c;
  char* ma_map=malloc(SIZE*sizeof(char));
  int compt=0;
  FILE * fichier = fopen(nomFichier, "r");
  
  do
  {
      c = fgetc(fichier); 
      if(c!='\n')
      {
	ma_map[compt]=c;
	compt++;
      }
  } while (c != EOF); 

  fclose(fichier);
  return ma_map;
}


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
    int ncolors, i, j, z, idx;
    float w;
    printf("fonction draw_screen\n");
    /* map*/
    map=lireMap("map/map.txt");
    printf("map lu\n");
    SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
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
                        
        }
    }
   
    for (i=0; i<w; i++) 
    {
      float t;
      float ca = (1.-i/w) * (a2-fov/2.) + i/w*(a2+fov/2.);
      for (t=0; t<20; t+=.05)
      {
	int cx = x+cos(ca)*t;
        int cy = y+sin(ca)*t;
        putpixel(affichage, w+cx*16, cy*16, 0); 
        idx = cx+cy*mapl;
	if (map[idx]!=' ') 
	{
	  int h = affichage->h/t;
          tmp.w = 1;
          tmp.h = h;
          tmp.x = i;
          tmp.y = (affichage->h-h)/2;
          z = (idx%ncolors)*3;
          SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, colors[z], colors[z+1],colors[z+2]));
          break;
	  
	}
          
    }
        
    }
    SDL_Flip(affichage);
}


void deplacement(float a, SDL_Rect position)
{/*
    float nxx, nyy;
    int nx, ny;
    nxx = (position.x + position.x*cos(a+M_PI/2)*.01 + position.y*cos(a)*.01);
    nyy = (position.y + position.x*sin(a+M_PI/2)*.01 + position.y*sin(a)*.01);
    nx = nxx;
    ny = nyy;
  
  
  if (nx>=0 && nx<mapc && ny>=0 && ny<mapl && map[nx+ny*mapl]==' ')
  {
    position.x = nx;
    position.y = ny;
    
  }*/
  SDL_Flip(affichage);
  printf("déplacement\n");

}


void end()
{
  SDL_FreeSurface(affichage);
  SDL_Quit();
}
