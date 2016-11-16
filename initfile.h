#ifndef INITFILE_H
#define INITFILE_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define AFFICHAGE_WIDTH 800
#define AFFICHAGE_HEIGHT 600 
#define mapc 32
#define mapl 32
#define SIZE mapc*mapl+1
#define pos_base 16.5



char* lireMap(char* nomFichier);
void init_window();
void init_menu();
void gameover(int *mode);
void putpixel(int x, int y, Uint32 pixel);
Uint32 getpixel(int itex, int x, int y, SDL_Surface * textures) ;
void draw_minicarte();
void draw_screen();
void deplacement(float a, SDL_Rect position,int *mode);
void objet_cherche();
void portePalea();
void porteNalea();
void mapalea();
void difficulte(int niv_difficulte);
void choixdiffi ();
void creamap();
void level_sup();
void WIN(int *mode);
void end();
void full();
void initialisation();
void initsprite();
void afflevel();
void affobjet();
void personnage(int x ,int y);
#endif
