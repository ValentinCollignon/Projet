#ifndef INITFILE_H
#define INITFILE_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

void gameover();
void init_window();

void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel);

void init_menu();
void draw_screen(float a);
void end();
#endif
