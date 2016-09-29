#ifndef GAME_H
#define GAME_H
#include "game.c"
#include "initfile.c"
#include "endfile.c"
#include "event.c"

void test(int a);

void init_window();

void end();
void HandleEvent(SDL_Event event,int *game_over);

#endif