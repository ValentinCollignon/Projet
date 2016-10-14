#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <cassert>
#include "game.h"

const int mapw = 32;
const int maph = 32;
float x = 16.3;
float y = 16.2;
float a = 0;
const float fov = M_PI/3;

char map[maph*mapw+1] = "\
################################\
#                            # #\
# ######### ###### ##### ##### #\
# #       #      #     # #     #\
# # ##### ###### ##### # ##### #\
# ###   #            # #     # #\
#     ############## # ####### #\
#### ##                      # #\
#               ##           # #\
#                              #\
#                            # #\
#                            # #\
#                            # #\
#          # ################# #\
#          #                 # #\
#          # ### #### ###### # #\
#          # #        #    # # #\
#          #        # # #### # #\
#          # #### ### #      # #\
#                     ###### # #\
##################### ##   # # #\
#    #   #   #        #  #   # #\
# ##   #   #    # ############ #\
#################     #   #  # #\
#               ##### # # # ## #\
# ########### # #     # # #  # #\
# #         # ### ### # #### # #\
# # ######### #   #          # #\
# #           # # #  ######### #\
# ############# # #  #   #   # #\
#               # #    #   # # #\
################################";

// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html
void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel) {
    int bpp = theScreen->format->BytesPerPixel;
    Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x*bpp;
    for (int i=0; i<bpp; i++) {
        p[i] = ((Uint8*)&pixel)[i];
    }
}

void Game::draw() {
    SDL_FillRect(sdl_screen_, NULL, SDL_MapRGB(sdl_screen_->format, 255, 255, 255));

    const int colors[] = {127, 127, 127};
    int ncolors = sizeof(colors)/(sizeof(int)*3);

    int w = sdl_screen_->w/2;
    for (int i=0; i<mapw; i++) { // draw the map
        for (int j=0; j<maph; j++) {
            if (map[i+j*mapw]==' ') continue;
            SDL_Rect tmp;
            tmp.w = 16;
            tmp.h = 16;
            tmp.x = i*16 + w;
            tmp.y = j*16;
            int z = ((i+j*mapw)%ncolors)*3;
            SDL_FillRect(sdl_screen_, &tmp, SDL_MapRGB(sdl_screen_->format, colors[z], colors[z+1],colors[z+2]));
        }
    }

    for (int i=0; i<w; i++) { // draw the "3D" view + visibility cone
        float ca = (1.-i/float(w)) * (a-fov/2.) + i/float(w)*(a+fov/2.);
        for (float t=0; t<20; t+=.05) {
            float cx = x+cos(ca)*t;
            float cy = y+sin(ca)*t;
            putpixel(sdl_screen_, w+cx*16, cy*16, 0); // visibility cone at the map

            int idx = int(cx)+int(cy)*mapw;
            if (map[idx]!=' ') {
                int h = sdl_screen_->h/t;
                SDL_Rect tmp;
                tmp.w = 1;
                tmp.h = h;
                tmp.x = i;
                tmp.y = (sdl_screen_->h-h)/2;
                int z = (idx%ncolors)*3;
                SDL_FillRect(sdl_screen_, &tmp, SDL_MapRGB(sdl_screen_->format, colors[z], colors[z+1],colors[z+2]));
                break;
            }
        }
    }
    SDL_Flip(sdl_screen_);
}

Game::Game() : game_running_(false), strafe_(0), walk_(0) {
}

bool Game::init_sdl(const char* title, int width, int height, int bpp) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "SDL_Init failed, SDL_GetError()=" << SDL_GetError() << std::endl;
        return false;
    }
    SDL_WM_SetCaption(title, NULL);
    sdl_screen_ = SDL_SetVideoMode(width, height, bpp, 0);
    if (!sdl_screen_) {
        std::cerr << "SDL_SetVideoMode failed, SDL_GetError()=" << SDL_GetError() << std::endl;
        return false;
    }
    game_running_ = true;
    width_ = width;
    height_ = height;
    bpp_ = bpp;

    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);

    return true;
}

bool Game::running() {
        return game_running_;
}

void Game::handle_events() {
    SDL_Event event;
    
    if (SDL_PollEvent(&event)) {
        game_running_ = !(SDL_QUIT==event.type || (SDL_KEYDOWN==event.type && SDLK_ESCAPE==event.key.keysym.sym));
    switch (event.type) 
  {
    /* close button clicked */
      break;
    /* handle the keyboard */
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) 
      {
	case SDLK_q:
	case SDLK_LEFT:
	  strafe_ = -1;
	  break;
	case SDLK_d:
	case SDLK_RIGHT:
	  strafe_ =  1;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  walk_ = -1;
	  break;
	case SDLK_z:
	case SDLK_UP:
	  walk_ =  1;
	  break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) 
      {
	case SDLK_q:
	case SDLK_LEFT:
	  strafe_ = 0;
	  break;
	case SDLK_d:
	case SDLK_RIGHT:
	  strafe_ =  0;
	  break;
	case SDLK_s:
	case SDLK_DOWN:
	  walk_ = 0;
	  break;
	case SDLK_z:
	case SDLK_UP:
	  walk_ =  0;
	  break;
      }
      break;
    case SDL_MOUSEMOTION:
      a += event.motion.xrel * .01;
      break;
  }
    /*if (SDL_PollEvent(&event)) {
        game_running_ = !(SDL_QUIT==event.type || (SDL_KEYDOWN==event.type && SDLK_ESCAPE==event.key.keysym.sym));
        if (SDL_KEYUP==event.type) {
            if ('q'==event.key.keysym.sym || 'd'==event.key.keysym.sym) strafe_ = 0;
            if ('z'==event.key.keysym.sym || 's'==event.key.keysym.sym) walk_   = 0;
        }
        if (SDL_KEYDOWN==event.type) {
            if ('d'==event.key.keysym.sym) strafe_ =  1;
            if ('q'==event.key.keysym.sym) strafe_ = -1;
            if ('z'==event.key.keysym.sym) walk_ =  1;
            if ('s'==event.key.keysym.sym) walk_ = -1;
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            a += event.motion.xrel * .01;
        }*/
    }

    // TODO proper delays

    float nx = (x + strafe_*cos(a+M_PI/2)*.01 + walk_*cos(a)*.01);
    float ny = (y + strafe_*sin(a+M_PI/2)*.01 + walk_*sin(a)*.01);

    if (int(nx)>=0 && int(nx)<mapw && int(ny)>=0 && int(ny)<maph && map[int(nx)+int(ny)*mapw]==' ') {
        x = nx;
        y = ny;
    }
}

void Game::clean() {
    if (sdl_screen_) SDL_FreeSurface(sdl_screen_);
    SDL_Quit();
}

