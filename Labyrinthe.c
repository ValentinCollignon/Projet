#include "event.h"
#include "initfile.h"
#include <SDL.h>



int main()
{
    int game_over=0;
    int mode =0;
    init_window();
    SDL_Rect depl;
    depl.x = 16.9;
    depl.y = 16.9;
    
    while(game_over != 1)
    {
        SDL_Event event;
        /* look for an event*/ 
        if(SDL_PollEvent(&event))
        {
            HandleEvent(event,&game_over,&mode, depl);
            
        }
        if (mode==1)
	{
	  draw_screen();
	}
    }
    end();
    return EXIT_SUCCESS;   
}
