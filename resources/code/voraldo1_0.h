#ifndef VORALDO
#define VORALDO

#include "includes.h"

//╦  ╦┌─┐┬─┐┌─┐┬  ┌┬┐┌─┐
//╚╗╔╝│ │├┬┘├─┤│   │││ │
// ╚╝ └─┘┴└─┴ ┴┴─┘─┴┘└─┘

class voraldo
{
  public:

    voraldo();
    ~voraldo();

  private:
    //  ╔═╗╦    ╦ ╦┬┌┐┌┌┬┐┌─┐┬ ┬
    //  ║ ╦║    ║║║││││ │││ ││││
    //  ╚═╝╩═╝  ╚╩╝┴┘└┘─┴┘└─┘└┴┘
    SDL_Window * window;
    SDL_GLContext GLcontext;


    //  ╔═╗╦    ╔╦╗┌─┐┌┬┐┌─┐
    //  ║ ╦║     ║║├─┤ │ ├─┤
    //  ╚═╝╩═╝  ═╩╝┴ ┴ ┴ ┴ ┴

    //texture handles, details tbd
    ImVec4 clear_color;

    //  ╦  ╦┌─┐─┐ ┬┌─┐┬    ╔╗ ┬  ┌─┐┌─┐┬┌─
    //  ╚╗╔╝│ │┌┴┬┘├┤ │    ╠╩╗│  │ ││  ├┴┐
    //   ╚╝ └─┘┴ └─└─┘┴─┘  ╚═╝┴─┘└─┘└─┘┴ ┴

      //TBD

    //  ╔╦╗┌─┐┌┐┌┬ ┬  ╔═╗┌┬┐┌─┐┌┬┐┌─┐
    //  ║║║├┤ ││││ │  ╚═╗ │ ├─┤ │ ├┤
    //  ╩ ╩└─┘┘└┘└─┘  ╚═╝ ┴ ┴ ┴ ┴ └─┘
    menu_state current_menu_state;


    //  ╦┌┐┌┬┌┬┐  ╔═╗┬ ┬┌┐┌┌─┐┌┬┐┬┌─┐┌┐┌┌─┐
    //  ║││││ │   ╠╣ │ │││││   │ ││ ││││└─┐
    //  ╩┘└┘┴ ┴   ╚  └─┘┘└┘└─┘ ┴ ┴└─┘┘└┘└─┘
    void create_window();
    void gl_data_setup();


    //  ╔╦╗┌─┐┬┌┐┌  ╦  ┌─┐┌─┐┌─┐  ╔═╗┬ ┬┌┐┌┌─┐┌┬┐┬┌─┐┌┐┌┌─┐
    //  ║║║├─┤││││  ║  │ ││ │├─┘  ╠╣ │ │││││   │ ││ ││││└─┐
    //  ╩ ╩┴ ┴┴┘└┘  ╩═╝└─┘└─┘┴    ╚  └─┘┘└┘└─┘ ┴ ┴└─┘┘└┘└─┘
    void draw_menu_and_take_input();


    //  ╔═╗ ┬ ┬┬┌┬┐
    //  ║═╬╗│ ││ │
    //  ╚═╝╚└─┘┴ ┴
    void quit();  //this destroys the windows, creates a
    // fullscreen exit splash and then exits the program

};

#endif
