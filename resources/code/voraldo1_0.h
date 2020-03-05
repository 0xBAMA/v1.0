#ifndef VORALDO
#define VORALDO

#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;

using std::flush;
using std::endl;

#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#include "../glm/glm.hpp" //general vector types
#include "../glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "../glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "../glm/gtx/transform.hpp"

#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengl.h>     //allows you to run OpenGL inside of SDL2
#include <SDL2/SDL_ttf.h>   //allows rendering of text using .ttf font files (SDL 2d renderer only)

//png loading library - very powerful
#include "lodepng.h"

//shader compilation wrapper - may need to be extended
#include "shader.h"

//up to 3d perlin noise generation
#include "perlin.h"

//enum for the menu states
#include "menu_states.h"


//╦  ╦┌─┐┬─┐┌─┐┬  ┌┬┐┌─┐
//╚╗╔╝│ │├┬┘├─┤│   │││ │
// ╚╝ └─┘┴└─┴ ┴┴─┘─┴┘└─┘

class voraldo
{
  public:

    voraldo();
    ~voraldo();

  private:
    //╔═╗╦    ╦ ╦┬┌┐┌┌┬┐┌─┐┬ ┬
    //║ ╦║    ║║║││││ │││ ││││
    //╚═╝╩═╝  ╚╩╝┴┘└┘─┴┘└─┘└┴┘
    SDL_Window * OpenGL_window;
    SDL_GLContext GLcontext;

    void create_gl_window();
    void update_gl_window();

    // //╔═╗╔╦╗╦    ╦ ╦┬┌┐┌┌┬┐┌─┐┬ ┬
    // //╚═╗ ║║║    ║║║││││ │││ ││││
    // //╚═╝═╩╝╩═╝  ╚╩╝┴┘└┘─┴┘└─┘└┴┘
    // SDL_Window * SDL_window;
    // SDL_Renderer* SDL_2D_renderer;
    // int sdl_win_x, sdl_win_y;
    //
    // void create_sdl_window();
    // void update_sdl_window();

    // //╔═╗┌─┐┌┐┌┌┬┐
    // //╠╣ │ ││││ │
    // //╚  └─┘┘└┘ ┴
    // TTF_Font *font;
    //
    // void sdl_ttf_init();
    // void ttf_string(std::string s, int basex, int basey, unsigned char r, unsigned char g, unsigned char b);

    //╔╦╗┌─┐┌┐┌┬ ┬  ╔═╗┌┬┐┌─┐┌┬┐┌─┐
    //║║║├┤ ││││ │  ╚═╗ │ ├─┤ │ ├┤
    //╩ ╩└─┘┘└┘└─┘  ╚═╝ ┴ ┴ ┴ ┴ └─┘
    menu_state current_menu_state;


    //╔╦╗┌─┐┬┌┐┌  ╦  ┌─┐┌─┐┌─┐  ╔═╗┬ ┬┌┐┌┌─┐┌┬┐┬┌─┐┌┐┌┌─┐
    //║║║├─┤││││  ║  │ ││ │├─┘  ╠╣ │ │││││   │ ││ ││││└─┐
    //╩ ╩┴ ┴┴┘└┘  ╩═╝└─┘└─┘┴    ╚  └─┘┘└┘└─┘ ┴ ┴└─┘┘└┘└─┘
    void handle_input();
    void draw_menu();

    // void update_windows();

    void quit();  //this destroys the windows, creates a
    // fullscreen exit splash and then exits the program

};

#endif
