#include <SDL.h>
#include "initfile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define mapc 32
#define mapl 32
#define SIZE mapc*mapl+1

const int colors[] = {120, 120, 120};
const float fov = M_PI/3;
float x ;
float y ;
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
  x = 16.3;
  y = 16.3;
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
    /*printf("x=%f\ny=%f\n",x,y);
    map*/
    map=lireMap("map/map.txt");
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
            if (map[i+j*mapl]=='#')
	    {
            tmp.x = i*16 + w;
            tmp.y = j*16;
            z = ((i+j*mapl)%ncolors)*3;
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, colors[z], colors[z+1],colors[z+2]));
	    }
	    if (map[i+j*mapl]=='+')
	    {
            tmp.x = i*16 + w;
            tmp.y = j*16;
            z = ((i+j*mapl)%ncolors)*3;
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 255, 0,0));
	    }   
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
	if (map[idx]=='#') 
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
	if (map[idx]=='+') 
	{
	  int h = affichage->h/t;
          tmp.w = 1;
          tmp.h = h;
          tmp.x = i;
          tmp.y = (affichage->h-h)/2;
          z = (idx%ncolors)*3;
          SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 200, 200,100));
          break;
	}
          
    }
        
    }
    SDL_Flip(affichage);
}


void deplacement(float a, SDL_Rect position,int*mode)
{
    float nxx, nyy;
    int nx, ny;
    nxx = (position.x + position.x*cos(a+M_PI/2)*.01 + position.y*cos(a)*.01);
    nyy = (position.y + position.x*sin(a+M_PI/2)*.01 + position.y*sin(a)*.01);
    nx = x+nxx;
    ny = y+nyy;
    a2 = a;/*transfert de l'angle a dans initfile*/
  
  if (map[nx+ny*mapl]==' ')
  {
    printf("dans le if depl");
    x += nxx;
    y += nyy;
    
  }
  SDL_Flip(affichage);
  printf("déplacement\n");
  if (map[nx+ny*mapl]=='+')
  {
    WIN(mode);
  }

}

void WIN(int* mode)
{
  SDL_Surface *temp, *win;
  SDL_Rect rcwin;
  int colorkey;
  *mode = 0;
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rcwin.x = 0;
  rcwin.y = 0;
  temp  = SDL_LoadBMP("image/gameover.bmp");
  win = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_FreeSurface(affichage);
  SDL_SetColorKey(win, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  SDL_BlitSurface(win, NULL, affichage, &rcwin);

  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  printf("fonction win\n");
  SDL_Delay(2000);
  SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
  SDL_Flip(affichage);
  init_menu();
  
}

void end()
{
  SDL_FreeSurface(affichage);
  SDL_Quit();
}
