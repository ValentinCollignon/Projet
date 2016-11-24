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
void objet_cherche(char objet);
void portAlea();
void mapalea();
void difficulte(int niv_difficulte);
void choixdiffi ();
void creamap(int num_difficulte);
void level_sup();
void afflettre(char Lettre,int lx, int ly);
void affnum(int num,int lx, int ly);
void WIN(int *mode);
void end();
void full();
void initialisation();
SDL_Surface* init_sprite_(char *nomSprite);
void initsprite();
void afflevel();
void affobjet();

#endif
