#ifndef VORALDO
#define VORALDO

#include <stdio.h>

//stl includes
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

//iostream aliases
using std::cin;
using std::cout;
using std::cerr;

using std::flush;
using std::endl;

//vector math library GLM
#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#include "../glm/glm.hpp" //general vector types
#include "../glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "../glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "../glm/gtx/transform.hpp"

//not sure as to the utility of this
#define GL_GLEXT_PROTOTYPES


//GUI library (dear ImGUI)
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"


// Initialize glew loader with glewInit()
#include <GL/glew.h>

//SDL includes - windowing, gl context, system info
#include <SDL.h>
//allows you to run OpenGL inside of SDL2
#include <SDL_opengl.h>


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
    //  ╔═╗╦    ╦ ╦┬┌┐┌┌┬┐┌─┐┬ ┬
    //  ║ ╦║    ║║║││││ │││ ││││
    //  ╚═╝╩═╝  ╚╩╝┴┘└┘─┴┘└─┘└┴┘
    int total_screen_height;
    int total_screen_width;

    SDL_Window * window;
    SDL_GLContext GLcontext;


    //  ╔═╗╦    ╔╦╗┌─┐┌┬┐┌─┐
    //  ║ ╦║     ║║├─┤ │ ├─┤
    //  ╚═╝╩═╝  ═╩╝┴ ┴ ┴ ┴ ┴
    ImVec4 clear_color;


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
