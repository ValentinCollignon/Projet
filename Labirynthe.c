#include "endfile.h"
#include "event.h"
#include "initfile.h"
#include <SDL.h>
#define mapc 32
#define mapl 32

/*
char map[mapl*mapc+1] = "\
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
*/



int main()
{
    int game_over=0;
    init_window();
    while(game_over != 1)
    {
        SDL_Event event;
        /* look for an event */
        if(SDL_PollEvent(&event))
        {
            HandleEvent(event,&game_over);
            
        }
        
    }
    end();
    return EXIT_SUCCESS;   
}
