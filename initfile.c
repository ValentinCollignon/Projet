#include <SDL.h>
#include "initfile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



const int colors[] = {120, 120, 120, 200, 200, 200, 155, 155, 155};
const float fov = M_PI/3;
float x ;
float y ;
float a2=0;
int nombre_objet=0,level = 1,levelporteN = 1, compL = 1, comptPorteN = 0,colorkey,posxP=3,posyP=0,texsize,ntextures;
int obmap=0;
SDL_Rect rclettre , rcSrclettre, rcSrcpersonnage, rcpersonnage;

/*mise en place de la fenetre principale*/
SDL_Surface * affichage ,*lettre, *textures_, *objet_map, *text_sol;
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
  affichage = SDL_SetVideoMode(AFFICHAGE_WIDTH, AFFICHAGE_HEIGHT, 32, SDL_SWSURFACE);
  init_menu();
  initsprite();
}

void init_menu()
{
  SDL_Surface *temp, *menu;
  SDL_Rect rcmenu;
  x = pos_base;
  y = pos_base;
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

void gameover(int *mode)
{
    SDL_Surface *temp, *gamover;
    SDL_Rect rcgameover;
    colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
    rcgameover.x = 0;
    rcgameover.y = 0;
    temp  = SDL_LoadBMP("image/game_over3.bmp");
    gamover = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_FreeSurface(affichage);
    SDL_SetColorKey(gamover, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    SDL_BlitSurface(gamover, NULL, affichage, &rcgameover);
    SDL_UpdateRect(affichage, 0, 0, 0, 0);
    printf("fonction gameover\n");
    SDL_Delay(3000);
    *mode = 0;
    initialisation();
    init_menu();
  
} 

void putpixel(int x, int y, Uint32 pixel) 
{
  if (x<0 || y<0 || x>=affichage->w || y>=affichage->h) return;
    int bpp = affichage->format->BytesPerPixel;
    Uint8 *p = (Uint8*)affichage->pixels + y * affichage->pitch + x*bpp;
    int i;
    for (i=0; i<bpp; i++) 
    {
        p[i] = ((Uint8*)&pixel)[i];
    }
}

Uint32 getpixel(int itex, int x, int y, SDL_Surface * textures) 
{
    texsize = textures->h;
    ntextures = textures->w/texsize ;
    
    if (itex<0 || itex>=ntextures || x<0 || y<0 || x>=texsize || y>=texsize) return 0;
    int bpp = textures->format->BytesPerPixel;
    /*Uint8 *p = (Uint8 *)textures->pixels + y * textures->pitch + x * bpp;*/
    Uint8 *p = (Uint8 *)textures->pixels + y*textures->pitch + (x+texsize*itex)*bpp;
    return p[0] | p[1] << 8 | p[2] << 16;
}

void draw_minicarte()
{
  SDL_Rect tmp;
  int i,j;
  float w;
  if (obmap ==0)
  {
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
  }
    
    putpixel( (w)+1+x*8, y*8, 0);
    putpixel( (w)+x*8, y*8, 0);
    putpixel( (w)+x*8, 1+y*8, 0);
    putpixel( (w)+x*8+1, y*8+1, 0);
    SDL_Flip(affichage);
}

void draw_screen()
{
    
    int  i,j , z, idx, aff;
    float w;
     /*map*/
    SDL_Rect ciel;
    ciel.h = 300;
    ciel.w = 800;
    ciel.x = 0;
    /*SDL_FillRect(affichage,&ciel , SDL_MapRGB(affichage->format, 119, 181, 254));
    ciel.y = 300;
    SDL_FillRect(affichage,&ciel , SDL_MapRGB(affichage->format, 0, 181, 0));*/
    w = affichage->w;
    for(i =0; i<600; i++)
    {
        float t;
        for(t=0;t<w;t+=0.5){
            putpixel(i,(affichage->w), getpixel(0 ,i, (t*64)/w,text_sol));
      }
    }
    
    
    
 
    /*draw map*/
    w = affichage->w;

    for (i=0; i<w; i++) 
    {
      float t;
      float ca = (1.-i/w) * (a2-fov/2.) + i/w*(a2+fov/2.);
      int h = affichage->h/t;

      for (t=0; t<20; t+=.05)
      {
	float cx = x+cos(ca)*t;
        float cy = y+sin(ca)*t;
	int cxx=cx;
	int cyy=cy;
        idx = cxx+cyy*mapl;
	int tx = fmax(fabs(cx-floor(cx+.5)), fabs(cy-floor(cy+.5)))*texsize; 
        int ty;
        
                

	if (map[idx]=='O') 
	{
            for (ty=0; ty<(h); ty++) 
	    { 
                putpixel(i, ty+(affichage->h-h)/2, getpixel(0, tx, (ty*64)/h,objet_map));
            }

	  break;
	}
	else
	{
	  
	  if (((map[idx]=='+') || (map[idx]=='-')) &&( nombre_objet == 0) ) 
	  {
            for (ty=0; ty<h; ty++) { 
                 putpixel(i, ty+(affichage->h-h)/2, getpixel(3, tx, (ty*64)/h,textures_));
            }
	    break;
	  }
	  if (map[idx]=='M') 
	  {
            for (ty=0; ty<(h); ty++) { 
                putpixel(i, ty+(affichage->h-h)/2, getpixel(0, tx, (ty*256)/h,objet_map));
            }

	    break;
	  }
	  if ((map[idx]!=' ') && ( nombre_objet != 0) || ((map[idx]=='#') && ( nombre_objet == 0)))
	  {
            for (ty=0; ty<h; ty++) { 
	      putpixel(i, ty+(affichage->h-h)/2, getpixel(2, tx, (ty*64)/h,textures_));
            }
	    break;
	  }
	}
	
          
    }


        
    }
    
    afflevel();
    affobjet();
    /*personnage(posxP,posyP);*/
    SDL_Flip(affichage);
}

void deplacement(float a, SDL_Rect position,int*mode)
{
    float nxx, nyy;
    int nx, ny, i;
    nxx = (position.x*cos(a+M_PI/2)*.1 + position.y*cos(a)*.1);
    nyy = (position.x*sin(a+M_PI/2)*.1 + position.y*sin(a)*.1);
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
    comptPorteN ++;
    if (comptPorteN == 2)
    {
      gameover(mode);
      
    }
    else
    {
      for (i = 1 ;i <= level; i++)
      {
	objet_cherche();
	
      }
    }
  }
  if (map[nx+ny*mapl]=='O')
  {
    x += nxx;
    y += nyy;
    nombre_objet -= 1;
    map[nx+ny*mapl]=' ';
    
  }
  if (map[nx+ny*mapl]=='M')
  {
    x += nxx;
    y += nyy;
    obmap -= 1;
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

void objet_cherche_map()
{
  int xob=0;
  int yob=0;
  
  srand(time(NULL));
  while (map[xob+yob*mapl]!=' ')
  {
    xob=rand()%(32);
    yob=rand()%(32);
  }
  obmap += 1;
  map[xob+yob*mapl]='M';
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
  nummap=rand()%(3);
  switch (nummap)
  {
    case 2:
      map = lireMap("map/map_3.tkt");
      /*normal*/
      break;
    case 1:
      map=lireMap("map/map.txt");
      /*difficile*/
      break;
      
    case 0:
      map=lireMap("map/map_2.txt");
      /*facile*/
      break;
  }
}

void difficulte(int niv_difficulte)
{
  switch(niv_difficulte)
  {
    case 0:
      map=lireMap("map/map_2.txt");
      break;
    case 1:
      map = lireMap("map/map_3.tkt");
      break;
    case 2:
      map = lireMap("map/map.txt");
      break;
    case 3:
      mapalea();
      break;
    
  }

}

void choixdiffi ()
{
  printf("diff choix\n");
  SDL_Surface *temp, *diff;
  SDL_Rect rcdiff;
  SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
  x = pos_base;
  y = pos_base;
  rcdiff.x = 128;
  rcdiff.y = 128;
  temp  = SDL_LoadBMP("image/facile.bmp");
  diff = SDL_DisplayFormat(temp);
  SDL_BlitSurface(diff, NULL, affichage, &rcdiff);
  
  rcdiff.x = AFFICHAGE_WIDTH/2 + 128;
  rcdiff.y = 128;
  temp  = SDL_LoadBMP("image/moyen.bmp");
  diff = SDL_DisplayFormat(temp);
  SDL_BlitSurface(diff, NULL, affichage, &rcdiff);
  
  rcdiff.x = 128;
  rcdiff.y = AFFICHAGE_HEIGHT/2 + 128;
  temp  = SDL_LoadBMP("image/dif.bmp");
  diff = SDL_DisplayFormat(temp);
  SDL_BlitSurface(diff, NULL, affichage, &rcdiff);
  
  rcdiff.x = AFFICHAGE_WIDTH/2+128;
  rcdiff.y = AFFICHAGE_HEIGHT/2 + 128;
  temp  = SDL_LoadBMP("image/alea.bmp");
  diff = SDL_DisplayFormat(temp);
  SDL_BlitSurface(diff, NULL, affichage, &rcdiff);
  
  
  
  SDL_FreeSurface(temp);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  
}

void creamap()
{
  int i;

  for (i = 1 ;i <= level; i++)
  {
    objet_cherche();
  }
  objet_cherche_map();
  portePalea();
  for (i=1;i<=(levelporteN);i++)
  {
    porteNalea();
  }
}

void level_sup()
{
  SDL_Surface *temp, *levelsup;
  SDL_Rect rclevelsup;
  int colorkey;
  x = pos_base;
  y = pos_base;
  level +=1;
  if (level%5 == 0)
  {
    levelporteN = levelporteN * 2 + compL;
    compL++;
  }
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rclevelsup.x = 0;
  rclevelsup.y = 0;
  temp  = SDL_LoadBMP("image/level_sup.bmp");
  levelsup = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_FreeSurface(affichage);
  SDL_SetColorKey(levelsup, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  SDL_BlitSurface(levelsup, NULL, affichage, &rclevelsup);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  printf("fonction level_sup\n");


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
  level_sup();

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

void initialisation()
{
 nombre_objet = 0;
 level = 1;
 levelporteN = 1;
 compL = 1;
 /*creamap();*/
}

void initsprite()
{
  SDL_Surface *temp;
  temp   = SDL_LoadBMP("spritealfanum/lettre.bmp");
  lettre = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  SDL_SetColorKey(lettre, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  rcSrclettre.w = 24;
  rcSrclettre.h = 24;
  objet_map = SDL_LoadBMP("image/objet_carte.bmp");
  textures_ = SDL_LoadBMP("image/walltext.bmp");
  text_sol = SDL_LoadBMP("image/sol.bmp");

  
  
}

void afflevel()
{
  int i=20;
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4);
  rclettre.y = 10;
  rcSrclettre.x = rcSrclettre.w*12;
  rcSrclettre.y = rcSrclettre.w*4;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+i;
  rcSrclettre.x = rcSrclettre.w*5;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+2*i;
  rcSrclettre.x = rcSrclettre.w*6;
  rcSrclettre.y = rcSrclettre.w*7;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+3*i;
  rcSrclettre.x = rcSrclettre.w*5;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+4*i;
  rcSrclettre.x = rcSrclettre.w*12;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+5*i;
  rcSrclettre.x = rcSrclettre.w*10;
  rcSrclettre.y = rcSrclettre.w*3;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+6*i;
  switch (level/10)
  {
    case 0:
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 1:
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 2:
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
  }
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+7*i;
  switch (level%10)
  {
    case 0:
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 1:
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 2:
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 3:
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 4:
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 5:
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 6:
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 7:
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 8:
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
    
    case 9:
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
  }
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
}

void affobjet()
{
  int i=20;
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4);
  rclettre.y = 34;
  rcSrclettre.x = rcSrclettre.w*15;
  rcSrclettre.y = rcSrclettre.w*4;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+i;
  rcSrclettre.x = rcSrclettre.w*2;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+2*i;
  rcSrclettre.x = rcSrclettre.w*10;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+3*i;
  rcSrclettre.x = rcSrclettre.w*5;
  rcSrclettre.y = rcSrclettre.w*6;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+4*i;
  rcSrclettre.x = rcSrclettre.w*4;
  rcSrclettre.y = rcSrclettre.w*7;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+5*i;
  rcSrclettre.x = rcSrclettre.w*10;
  rcSrclettre.y = rcSrclettre.w*3;
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+6*i;
  switch (nombre_objet/10)
  {
    case 0:
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 1:
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 2:
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
  }
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+7*i;
  switch (nombre_objet%10)
  {
    case 0:
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 1:
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 2:
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 3:
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 4:
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 5:
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 6:
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 7:
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
      
    case 8:
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
    
    case 9:
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*3;
      break;
  }
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
}

void personnage(int x, int y)
{
  posxP = x;
  posyP = y;
  SDL_Surface *temp, *personnage;
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rcSrcpersonnage.w = 144;
  rcSrcpersonnage.h = 192;
  rcSrcpersonnage.x = rcSrcpersonnage.w * posyP;
  rcSrcpersonnage.y = rcSrcpersonnage.h * posxP;
  rcpersonnage.x = (AFFICHAGE_WIDTH/2) - (rcSrcpersonnage.w/2);
  rcpersonnage.y = AFFICHAGE_HEIGHT - rcSrcpersonnage.h;
  temp  = SDL_LoadBMP("image/loup.bmp");
  personnage = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_SetColorKey(personnage, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  SDL_BlitSurface(personnage, &rcSrcpersonnage, affichage, &rcpersonnage);
  SDL_Flip(affichage);

}
  
