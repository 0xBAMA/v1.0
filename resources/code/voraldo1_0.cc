#include "voraldo1_0.h"
#include "debug.h"  //for some reason, this has to be included here rather than in the .h file

voraldo::voraldo()
{
  SDL_Init( SDL_INIT_EVERYTHING );    //initialize SDL library
  current_menu_state = MAIN_MENU;     //initial state of the menu

  sdl_ttf_init();                     //initialize font rendering utility

  create_gl_window();                 //create the window that plays host to our OpenGL context
  create_sdl_window();                //create the window that plays host to the menu

  gl_debug_enable();                  //enable OpenGL debug output

  while(current_menu_state != EXIT)
  {
    draw_menu();    //draw first, to avoid drawing a frame of the wrong menu when entering the EXIT state
    // take_input();   //this is where any state changes are going to happen

    SDL_Delay(10);
  }

}

voraldo::~voraldo()
{
  quit();
}
