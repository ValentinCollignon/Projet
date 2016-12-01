#include "event.h"
#include "initfile.h"
#include <SDL.h>



int main()
{
    int game_over = 0;
    int mode = 0;
    /* *
     * mode 0 : menu ou pause 
     * mode 1 : jeu
     * mode 2 : choix difficultée
     * mode 3 : level sup
     * mode 4 : carte 
     * mode 5 : option
     * */
    
    
    init_window();
    
    /*repetition des evenement maintenue*/
    SDL_EnableKeyRepeat(5, 5);
    
    
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
