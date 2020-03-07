#include "voraldo1_0.h"
#include "debug.h"  //for some reason, this has to be included here rather than in the .h file

voraldo::voraldo()
{
  current_menu_state = MAIN_MENU;     //initial state of the menu

  create_window();                    //create the window that plays host to our OpenGL context
  gl_data_setup();                    //compile shaders, load textures
  gl_debug_enable();                  //enable OpenGL debug output

  while(current_menu_state != EXIT)   //loop - main returns 0 when constructor is done
  {
    draw_menu_and_take_input();       //clear screen, gl output, present imgui, make any state changes
  }
}

voraldo::~voraldo()
{
  quit();                             //deallocate gl data, SDL and dear ImGUI exit functions
}
