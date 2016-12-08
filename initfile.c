#include <SDL.h>
#include "initfile.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const float fov = M_PI/3;
float x ;
float y ;
float a2=0;
int nombre_objet=0,level = 1,levelporteN = 1, compL = 1, comptPorteN = 0,colorkey,posxP=3,posyP=0,texsize,ntextures;
int obmap=0;
SDL_Rect rclettre , rcSrclettre, rcSrcpersonnage, rcpersonnage;
SDL_Surface * affichage ,*lettre, *textures_, *objet_map,*objet_a_chercher, *win, *menu,*mur;
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
	  ma_map[compt]=' ';
	  if (c != ' ')
	    {
	      ma_map[compt]='#';
	    }
	  compt++;
	  
	}
    } while (c != EOF); 
  
  fclose(fichier);
  return ma_map;
}

/*mise en place de la fenetre principale*/
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
  SDL_Rect rcmenu;
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rcmenu.x = 0;
  rcmenu.y = 0;
  menu = init_sprite_("image/menu.bmp");
  SDL_SetColorKey(menu, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  SDL_BlitSurface(menu, NULL, affichage, &rcmenu);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  SDL_FreeSurface(menu);
}

void WIN_game_over(int* mode, int w_g_o) /*w_g_o : bouleen win=0,  game over=!0*/
{
  SDL_Surface *win;
  SDL_Rect rcwin;
  int colorkey;
  *mode = 0;
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  rcwin.x = 0;
  rcwin.y = 0;
  if(w_g_o == 0)
    {
      win = init_sprite_("image/win.bmp"); 
    }
  else
    {
      win = init_sprite_("image/game_over3.bmp");
    }
  SDL_SetColorKey(win, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  SDL_BlitSurface(win, NULL, affichage, &rcwin);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  SDL_Delay(2000);
  SDL_FillRect(affichage, NULL, SDL_MapRGB(affichage->format, 255, 255, 255));
  SDL_Flip(affichage);
  SDL_FreeSurface(win);
  initialisation();
  init_menu();
}

void putpixel(int x, int y, Uint32 pixel) 
{
  int i, bpp;
  Uint8 *p;
  if (x<0 || y<0 || x>=affichage->w || y>=affichage->h) return;
  bpp = affichage->format->BytesPerPixel;
  p = (Uint8*)affichage->pixels + y * affichage->pitch + x*bpp;
  for (i=0; i<bpp; i++) 
    {
      p[i] = ((Uint8*)&pixel)[i];
    }
}

Uint32 getpixel(int itex, int x, int y, SDL_Surface * textures) 
{
  int bpp;
  Uint8 *p;
  texsize = textures->h;
  ntextures = textures->w/texsize ;
  if (itex<0 || itex>=ntextures || x<0 || y<0 || x>=texsize || y>=texsize) return 0;
  bpp = textures->format->BytesPerPixel;
  p = (Uint8 *)textures->pixels + y*textures->pitch + (x+texsize*itex)*bpp;
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
		  SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 0,128,0));
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
		  SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 255,0,0));
		}
	    }
	}
      tmp.x = x*8 + w;
      tmp.y = y*8;
      SDL_FillRect(affichage, &tmp, SDL_MapRGB(affichage->format, 255, 0,255));
      SDL_Flip(affichage);
    }
}

void draw_texture(SDL_Surface *nom_texture, int h, int tx, int i)
{
  int ty;
  for (ty=0; ty<(h); ty++) 
    { 
      putpixel(i, ty+(affichage->h-h)/2, getpixel(0, tx, (ty*nom_texture->h)/h,nom_texture));
    }
}

void draw_screen()
{
  int  i, idx, tx, cyy, cxx, h ,z;
  float w, cx, cy, t, ca;
  /*map*/
  SDL_Rect ciel, sol;
  ciel.h = 5;
  ciel.w = 800;
  ciel.x = 0;
  ciel.y = 0;
  sol.h = 5;
  sol.y = 300;
  sol.w = 800;
  sol.x = 0;
  for(z = 0; z < 200; z++)
    {
      SDL_FillRect(affichage,&ciel , SDL_MapRGB(affichage->format, 50+ z,5+z, 255 - z));
      ciel.y += ciel.h;
    }
  for(z=0; z < 200; z++)
    {
      SDL_FillRect(affichage,&sol , SDL_MapRGB(affichage->format, 0, 55+z , 54));
      sol.y += sol.h;
    }
  /*draw map*/
  w = affichage->w;
  
  for (i=0; i<w; i++) 
    {
      ca = (1.-i/w) * (a2-fov/2.) + i/w*(a2+fov/2.);
      
      for (t=0; t<20; t+=.05)

	{
	  h = affichage->h/t;
	  cx = x+cos(ca)*t;
	  cy = y+sin(ca)*t;
	  cxx=cx;
	  cyy=cy;
	  idx = cxx+cyy*mapl;
	  tx = fmax(fabs(cx-floor(cx+.1)), fabs(cy-floor(cy+.1)))*texsize; 
	  
	  if (map[idx]=='O') 
	    {
	      draw_texture(objet_a_chercher,h,tx,i);
	      break;
	    }
	  else
	    {
	      
	      if (((map[idx]=='+') || (map[idx]=='-')) &&( nombre_objet == 0) ) 
		{
		  draw_texture(textures_,h,tx,i);
		  break;
		}
	      else
		{
		  if (map[idx]=='M') 
		    {
		      draw_texture(objet_map,h,tx,i);
		      break;
		    }
		  else
		    {
		      if (((map[idx]!=' ') && ( nombre_objet != 0)) || ((map[idx]=='#') && ( nombre_objet == 0)))
			{
			  draw_texture(mur,h,tx,i);
			  break;
			}
		    }
		}
	    }     
	}
    }
  
  aff_param(0); /*affiachage level*/
  aff_param(1); /*affichage objet */
  aff_param(2); /*affichage vie */
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
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
      if(level < 20)
	{
	  level_sup(mode);
	}
      else
	{
	  WIN_game_over(mode,0);
	}
    }
  if ((map[nx+ny*mapl]=='-') && (nombre_objet == 0))
    {
      x = pos_base;
      y = pos_base ;
      position_initial(x,y);
      comptPorteN ++;
      if (comptPorteN == 2)
	{
	  WIN_game_over(mode, 1);
	}
      else
	{
	  for (i = 1 ;i <= level; i++)
	    {
	      objet_cherche('O');
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

void objet_cherche(char objet)
{
  int xob=0;
  int yob=0;
  srand(time(NULL));
  while (map[xob+yob*mapl]!=' ')
    {
      xob=rand()%(32);
      yob=rand()%(32);
    }
  if(objet == 'O')
    {
      nombre_objet += 1;
    }
  else
    if(objet == 'M')
      {
	obmap += 1; 
      }
  map[xob+yob*mapl] = objet;

}

void porteAlea(char porte)
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
  map[xp+yp*mapl]= porte;
}

void mapalea()
{
  int nummap;
  srand(time(NULL));
  nummap=rand()%(6);
  switch (nummap)
    {
    case 5:
      map = lireMap("map/map_6.txt");
      break;
      
    case 4:
      map = lireMap("map/map_5.txt");
      break;
      
    case 3:
      map = lireMap("map/map_4.txt");
      break;
      
    case 2:
      map = lireMap("map/map_3.txt");
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
  int alea;
  srand(time(NULL));
  alea = rand()%(2);
  switch(niv_difficulte)
    {
    case 0:
      switch(alea)
	{
	case 0:
	  map = lireMap("map/map_2.txt");
	  break;
	case 1:
	  map = lireMap("map/map_4.txt");
	  break;
	}
      break;
      
    case 1:
      switch(alea)
	{
	case 0:
	  map = lireMap("map/map_5.txt");
	  break;
	case 1:
	  map = lireMap("map/map_3.txt");
	  break;
	}
      break;
      
    case 2:
      switch(alea)
	{
	case 0:
	  map = lireMap("map/map.txt");
	  break;
	case 1:
	  map = lireMap("map/map_6.txt");
	  break;
	}
      break;
      
    case 3:
      mapalea();
      break;
    }
}

void choixdiffi ()
{
  SDL_Rect rcdiff;
  SDL_Surface  *diff, *diff1, *diff2, *diff3, *fontdiff;
  
  fontdiff = init_sprite_("image/choix_diff.bmp");
  diff = init_sprite_("image/alea.bmp");
  diff1 = init_sprite_("image/dif.bmp");
  diff2 = init_sprite_("image/facile.bmp");
  diff3 = init_sprite_("image/moyen.bmp");
  rcdiff.x = 0;
  rcdiff.y = 0;
  SDL_BlitSurface(fontdiff, NULL, affichage, &rcdiff);
  x = pos_base;
  y = pos_base;
  rcdiff.x = 128;
  rcdiff.y = 128;
  SDL_BlitSurface(diff2, NULL, affichage, &rcdiff);
  
  rcdiff.x = AFFICHAGE_WIDTH/2 + 128;
  rcdiff.y = 128;
  SDL_BlitSurface(diff3, NULL, affichage, &rcdiff);
  
  rcdiff.x = 128;
  rcdiff.y = AFFICHAGE_HEIGHT/2+75 ;
  SDL_BlitSurface(diff1, NULL, affichage, &rcdiff);
  
  rcdiff.x = AFFICHAGE_WIDTH/2+128;
  rcdiff.y = AFFICHAGE_HEIGHT/2 +75;
  SDL_BlitSurface(diff, NULL, affichage, &rcdiff);
  
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  SDL_FreeSurface(diff);
  SDL_FreeSurface(diff1);
  SDL_FreeSurface(diff2);
  SDL_FreeSurface(diff3);
}

void creamap(int num_difficulte)
{
  int i;
  difficulte(num_difficulte);
  obmap = 0;
  x = pos_base;
  y = pos_base;
  position_initial(x,y);
  for (i = 1 ;i <= level; i++)
    {
      objet_cherche('O');
    }
  objet_cherche('M');
  porteAlea('+');
  for (i=1;i<=(levelporteN);i++)
    {
      porteAlea('-');
    }
 
 
}

/*prossedure inter level*/
void level_sup(int *mode)
{
  char* sup;
  char supprec;
  int i = 0 ,lx=0,ly=affichage->h/2;
  SDL_Surface *levelsup;
  SDL_Rect rclevelsup;
  *mode = 3;
  comptPorteN -= 1;
  
  
  /*prepare le level suivant*/
  x = pos_base;
  y = pos_base;
  level +=1;
  if (level%5 == 0)
    {
      levelporteN = levelporteN * 2 + compL;
      compL++;
    }
  
  rclevelsup.x = 0;
  rclevelsup.y = 0;
  levelsup = init_sprite_("image/level_sup.bmp");
  SDL_BlitSurface(levelsup, NULL, affichage, &rclevelsup);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  supprec = 't';
    
  /*afficher le message*/
  sup = " Pour passer au labyrinthe  ";
  
  while (i<50 && !(sup [i] == ' ' && supprec == ' '))
    {
      if (sup[i]!=' ')
	{
	  afflettre(sup[i],lx,ly);
	  SDL_UpdateRect(affichage, 0, 0, 0, 0);
	}
      lx += 20;
      supprec = sup[i];
      i++;
    }
  affnum(level/10,lx,ly);
  lx += 20;
  affnum(level%10,lx,ly);
  lx = 0;
  ly += 30;
  supprec='r';
  i=0;
  sup=" pressez ENTREE  ";
  while (i<50 && !(sup [i] == ' ' && supprec == ' '))
    {
      if (sup[i]!=' ')
	{
	  afflettre(sup[i],lx,ly);
	  SDL_UpdateRect(affichage, 0, 0, 0, 0);
	}
      lx += 20;
      if (i==30)
	{
	  lx = 0;
	  ly += 30;
	}
      supprec = sup[i];
      i++;
    }
}

/* prossedure qui affiche une lettre a une position donnée*/
void afflettre(char Lettre,int lx, int ly)
{
  /* coordonné de la lettre dans la fenetre*/
  rclettre.x = lx;
  rclettre.y = ly;
  
  /* choix de la lettre*/
  switch (Lettre)
    {
    case 'a':
      rcSrclettre.x = rcSrclettre.w;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'b':
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'c':
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'd':
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'e':
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'f':
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'g':
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'h':
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'i':
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'j':
      rcSrclettre.x = rcSrclettre.w*10;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'k':
      rcSrclettre.x = rcSrclettre.w*11;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'l':
      rcSrclettre.x = rcSrclettre.w*12;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'm':
      rcSrclettre.x = rcSrclettre.w*13;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'n':
      rcSrclettre.x = rcSrclettre.w*14;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'o':
      rcSrclettre.x = rcSrclettre.w*15;
      rcSrclettre.y = rcSrclettre.w*6;
      break;
    case 'p':
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'q':
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'r':
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 's':
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 't':
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'u':
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'v':
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'w':
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'x':
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'y':
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'z':
      rcSrclettre.x = rcSrclettre.w*10;
      rcSrclettre.y = rcSrclettre.w*7;
      break;
    case 'A':
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'B':
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'C':
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'D':
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'E':
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'F':
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'G':
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'H':
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'I':
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'J':
      rcSrclettre.x = rcSrclettre.w*10;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'K':
      rcSrclettre.x = rcSrclettre.w*11;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'L':
      rcSrclettre.x = rcSrclettre.w*12;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'M':
      rcSrclettre.x = rcSrclettre.w*13;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'N':
      rcSrclettre.x = rcSrclettre.w*14;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'O':
      rcSrclettre.x = rcSrclettre.w*15;
      rcSrclettre.y = rcSrclettre.w*4;
      break;
    case 'P':
      rcSrclettre.x = rcSrclettre.w*0;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'Q':
      rcSrclettre.x = rcSrclettre.w*1;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'R':
      rcSrclettre.x = rcSrclettre.w*2;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'S':
      rcSrclettre.x = rcSrclettre.w*3;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'T':
      rcSrclettre.x = rcSrclettre.w*4;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'U':
      rcSrclettre.x = rcSrclettre.w*5;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'V':
      rcSrclettre.x = rcSrclettre.w*6;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'W':
      rcSrclettre.x = rcSrclettre.w*7;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'X':
      rcSrclettre.x = rcSrclettre.w*8;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'Y':
      rcSrclettre.x = rcSrclettre.w*9;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case 'Z':
      rcSrclettre.x = rcSrclettre.w*10;
      rcSrclettre.y = rcSrclettre.w*5;
      break;
    case ':':
      rcSrclettre.x = rcSrclettre.w*10;
      rcSrclettre.y = rcSrclettre.w*3;
      break;     
    }
  
  SDL_BlitSurface(lettre, &rcSrclettre, affichage, &rclettre);
}

/* prossedure qui affiche un chiffre a une position donnée*/
void affnum(int num,int lx, int ly)
{
  /* coordonné du chiffre dans la fenetre*/
  rclettre.x=lx;
  rclettre.y = ly;
  
  /*choix du chifre*/
  switch (num)
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


void end()
{
  free(map);
  SDL_FreeSurface(mur);
  SDL_FreeSurface(lettre);
  SDL_FreeSurface(objet_map);
  SDL_FreeSurface(objet_a_chercher);
  SDL_FreeSurface(textures_);
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
  comptPorteN = 0;
}

SDL_Surface* init_sprite_(char *nomSprite)
{
  SDL_Surface *temp, *nom;
  temp = SDL_LoadBMP(nomSprite);
  nom = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  return nom;
}

void initsprite()
{
  lettre = init_sprite_("spritealfanum/lettre.bmp");
  colorkey = SDL_MapRGB(affichage->format, 255, 0, 255);
  SDL_SetColorKey(lettre, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  rcSrclettre.w = 24;
  rcSrclettre.h = 24;
  textures_ = init_sprite_("image/porte.bmp");
  objet_map = init_sprite_("image/objet_carte.bmp");
  objet_a_chercher = init_sprite_("image/obj.bmp");
  mur = init_sprite_("image/mur.bmp");
}

void option()
{
  SDL_Surface *option_;
  SDL_Rect rcoption;
  rcoption.x = 0;
  rcoption.y = 0;
  option_= init_sprite_("image/option.bmp");
  SDL_BlitSurface(option_, NULL, affichage, &rcoption);
  SDL_UpdateRect(affichage, 0, 0, 0, 0);
  SDL_FreeSurface(option_);
}

void affphrase(char *phrase,int px,int py)
{
  int i=0;
  char pprec ='t';
  
  while (i<50 && !(phrase [i] == ' ' && pprec == ' '))
    {
      if (phrase[i]!=' ')
	{
	  afflettre(phrase[i],px,py);
	}
      px += 20;
      pprec = phrase[i];
      i++;
    }
}

void aff_param(int param)
{
  int i=20,j,numero;
  char *phrase;
  if (param == 0)
    {
      phrase = "Level:  ";
      numero = level;
      rclettre.y = 10;
    }
  if (param == 1)
    {
      phrase = "Objet:  ";
      numero = nombre_objet;
      rclettre.y = 34;
    }
  if (param == 2)
    {
      phrase = "Vie:  ";
      numero = 2-comptPorteN;
      rclettre.y = 58;
    }
  
  rclettre.x=AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4);
  affphrase(phrase,rclettre.x,rclettre.y);
  
  j = AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+6*i;
  affnum(numero/10,j, rclettre.y);
  
  j = AFFICHAGE_WIDTH - (AFFICHAGE_WIDTH/4)+7*i;
  affnum(numero%10,j, rclettre.y);
  
}

void position_initial(float aX, float aY)
{
  int newX, newY;
  newX = aX;
  newY = aY;
  while (map[newX+newY*mapl] == '#')
    {
      newX = newX+1;
      if (newX == mapl-1)
	{
	  newX = 1;	     
	  newY ++ ;
	  if (newY == mapl-1)
	    {
	      newY = 1;		
	    }
	}
    }
  x = newX;
  y = newY;    
}
