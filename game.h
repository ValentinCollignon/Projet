#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

void init_window();

void end();
void HandleEvent(SDL_Event event,int *game_over);

#endif