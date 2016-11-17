#include "event.h"
#include "initfile.h"
#include <SDL.h>



int main()
{
    int game_over = 0;
    int mode = 0;
    init_window();
    
    /*repetition des evenement maintenue*/
    SDL_EnableKeyRepeat(7, 7);
    
    
    while(game_over != 1)
    {
        SDL_Event event;
        /* look for an event*/ 
        if(SDL_PollEvent(&event))
        {
            HandleEvent(event,&game_over,&mode);
            
        }
        if (mode==1)
	{
	  draw_screen();
	}
    }
    end();
    return EXIT_SUCCESS;   
}
