#include "voraldo1_0.h"

//this is where stuff like the init functions will all be defined

void voraldo::create_window()
{
    cout << "╦  ╦┌─┐┬─┐┌─┐┬  ┌┬┐┌─┐" << endl;
    cout << "╚╗╔╝│ │├┬┘├─┤│   │││ │" << endl;
    cout << " ╚╝ └─┘┴└─┴ ┴┴─┘─┴┘└─┘ v1.0" << endl;

  cout << endl << "Creating OpenGL window ...";

  if(SDL_Init( SDL_INIT_EVERYTHING ) != 0)
  {
      printf("Error: %s\n", SDL_GetError());
  }

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );

  //this is how you query the screen resolution
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);

  //pulling these out because I'm going to try to span the whole screen with
  //the window, in a way that's flexible on different resolution screens
  int total_screen_width = dm.w;
  int total_screen_height = dm.h;

  OpenGL_window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );
  GLcontext = SDL_GL_CreateContext( OpenGL_window );

  SDL_GL_MakeCurrent(OpenGL_window, GLcontext);

  glClearColor( 0.26, 0.16, 0.0, 1.0 );   //really excited by the fact imgui has hsv selectors that let you change this easily from inside the program
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( OpenGL_window );

  cout << " done." << endl;

}

void voraldo::draw_menu_and_take_input()
{

}

void voraldo::quit()
{
  //blow away all the OpenGL data

  //destroy window

  //fullscreen exit splash? maybe if it's a quick one - I really liked that glitchy blinking eye gif

  SDL_Quit();

  cout << "GOODBYE" << endl << endl; //last line of code before main's return statement
}
