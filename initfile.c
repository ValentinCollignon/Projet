#include <SDL.h>
#include "initfile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define mapc 32
#define mapl 32
#define SIZE mapc*mapl+1

const int colors[] = {120, 120, 120, 200, 200, 200, 155, 155, 155};
const float fov = M_PI/3;
float x ;
float y ;
float a2=0;
int nombre_objet=0;

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
  affichage = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
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

void draw_minicarte()
{
  SDL_Rect tmp;
  int i,j;
  float w;
  w = affichage->w/4;
  tmp.w = 8;
  tmp.h = 8;
  for (i=0; i<mapl; i++) 
    { 
        for (j=0; j<mapc; j++) 
        {
            if (map[i+j*mapl]!=' ')
	    {
            tmp.x = i*8 + w;
            tmp.y = j*8;
            
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 0,255,0));
	    }
	    if (((map[i+j*mapl]=='+') || (map[i+j*mapl]=='-')) && ( nombre_objet == 0))
	    {
            tmp.x = i*8 + w;
            tmp.y = j*8;
            
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 0, 0,0));
	    }   
	    if (map[i+j*mapl]=='O')
	    {
            tmp.x = i*8 + w;
            tmp.y = j*8;
            
            SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 0,0,255));
	    }
        }
    }
    
    putpixel(affichage, (w)+1+x*8, y*8, 0);
    putpixel(affichage, (w)+x*8, y*8, 0);
    putpixel(affichage, (w)+x*8, 1+y*8, 0);
    putpixel(affichage, (w)+x*8+1, y*8+1, 0);
    SDL_Flip(affichage);
}

void draw_screen()
{
    
    SDL_Rect tmp;
    int ncolors, i, j, z, idx;
    float w;
    /*map*/

    
    
    SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
    /*draw map*/
    ncolors = sizeof(colors)/(sizeof(int)*3);
    w = affichage->w;
    tmp.w = 16;
    tmp.h = 16;

    for (i=0; i<w; i++) 
    {
      float t;
      float ca = (1.-i/w) * (a2-fov/2.) + i/w*(a2+fov/2.);
      for (t=0; t<20; t+=.05)
      {
	int cx = x+cos(ca)*t;
        int cy = y+sin(ca)*t;
        idx = cx+cy*mapl;
	if (map[idx]=='O') 
	{
	  int h = affichage->h/t;
          tmp.w = 1;
          tmp.h = h;
          tmp.x = i;
          tmp.y = (affichage->h-h)/2;
          SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 0,0,255));
	  break;
	}
	else
	{
	  if (((map[idx]!=' ') && ( nombre_objet != 0)) || ((map[idx]=='#') && ( nombre_objet == 0)))
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
	  if (((map[idx]=='+') || (map[idx]=='-')) &&( nombre_objet == 0) ) 
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
        
    }
    SDL_Flip(affichage);
}

void deplacement(float a, SDL_Rect position,int*mode)
{
    float nxx, nyy;
    int nx, ny;
    nxx = (position.x*cos(a+M_PI/2)*.01 + position.y*cos(a)*.01);
    nyy = (position.x*sin(a+M_PI/2)*.01 + position.y*sin(a)*.01);
    nx = x + nxx;
    ny = y + nyy;
    a2 = a;/*transfert de l'angle a dans initfile*/
  
  if (map[nx+ny*mapl]==' ')
  {
    x += nxx;
    y += nyy;
    
  }
  if ((map[nx+ny*mapl]=='+') && (nombre_objet == 0))
  {
    WIN(mode);
  }
  if ((map[nx+ny*mapl]=='-') && (nombre_objet == 0))
  {
    x = 16.5;
    y = 16.5;
    objet_cherche();
    
  }
  if (map[nx+ny*mapl]=='O')
  {
    x += nxx;
    y += nyy;
    nombre_objet -= 1;
    map[nx+ny*mapl]=' ';
    
  }

}

void objet_cherche()
{
  int xob=0;
  int yob=0;
  
  srand(time(NULL));
  while (map[xob+yob*mapl]!=' ')
  {
    xob=rand()%(32);
    yob=rand()%(32);
  }
  nombre_objet += 1;
  map[xob+yob*mapl]='O';
}

void portePalea()
{
  int xp=16;
  int yp=16;
  srand(time(NULL));
  while (map[xp+yp*mapl]!='#')
  {
    xp=rand()%(32);
    yp=rand()%(32);
    if ((map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((xp == 0) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((xp == 31) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((yp == 0) && (map[xp+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((yp == 31) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if (((xp == 0) && (yp == 0)) || ((xp == 0) && (yp == 31)) || ((xp == 31) && (yp == 0)) || ((xp == 31) && (yp == 31)))
    {
      xp = 16;
      yp = 16;
    }
      
  }
  map[xp+yp*mapl]='+';
}

void porteNalea()
{
  int xp=16;
  int yp=16;
  
  srand(time(NULL));
  while (map[xp+yp*mapl]!='#')
  {
    xp=rand()%(32);
    yp=rand()%(32);
    if ((map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((xp == 0) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((xp == 31) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((yp == 0) && (map[xp+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[(xp+1)+yp*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if ((yp == 31) && (map[xp+yp*mapl]=='#') && (map[(xp-1)+yp*mapl]=='#') && (map[xp+(yp-1)*mapl]=='#') && (map[xp+(yp+1)*mapl]=='#'))
    {
      xp = 16;
      yp = 16;
    }
    if (((xp == 0) && (yp == 0)) || ((xp == 0) && (yp == 31)) || ((xp == 31) && (yp == 0)) || ((xp == 31) && (yp == 31)))
    {
      xp = 16;
      yp = 16;
    }
  }
  map[xp+yp*mapl]='-';
}

void mapalea()
{
  int nummap;
  srand(time(NULL));
  nummap=rand()%(2);
  switch (nummap)
  {
    case 0:
      map=lireMap("map/map.txt");
      break;
      
    case 1:
      map=lireMap("map/map_2.txt");
      break;
  }
}

void creamap()
{
  int i;
  mapalea();
  objet_cherche();
  portePalea();
  for (i=0;i<3;i++)
  {
    porteNalea();
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
  temp  = SDL_LoadBMP("image/win.bmp");
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

void full()
{
  SDL_WM_ToggleFullScreen (affichage);
}
