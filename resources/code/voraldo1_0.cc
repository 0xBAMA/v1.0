#include "voraldo1_0.h"
#include "debug.h"  //for some reason, this has to be included here rather than in the .h file

voraldo::voraldo()
{
  SDL_Init( SDL_INIT_EVERYTHING );    //initialize SDL library
  current_menu_state = MAIN_MENU;     //initial state of the menu

  create_window();                    //create the window that plays host to our OpenGL context

  gl_debug_enable();                  //enable OpenGL debug output

  while(current_menu_state != EXIT)
  {
    //draw_menu_and_take_input();
    SDL_Delay(10);
  }

  //fall through and exit

}

voraldo::~voraldo()
{
  quit();
}
