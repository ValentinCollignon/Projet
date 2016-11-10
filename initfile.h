#ifndef INITFILE_H
#define INITFILE_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

char* lireMap(char* nomFichier);
void init_window();
void init_menu();
void gameover();
void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel);
void draw_minicarte();
void draw_screen();
void deplacement(float a, SDL_Rect position,int *mode);
void objet_cherche();
void portePalea();
void porteNalea();
void mapalea();
void difficulte(int niv_difficulte);
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
