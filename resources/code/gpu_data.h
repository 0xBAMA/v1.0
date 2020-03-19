#ifndef GPU_DATA
#define GPU_DATA

#include "includes.h"

//╔═╗┌─┐┌─┐┌┐┌╔═╗╦    ╔═╗┌─┐┌┐┌┌┬┐┌─┐┬┌┐┌┌─┐┬─┐
//║ ║├─┘├┤ │││║ ╦║    ║  │ ││││ │ ├─┤││││├┤ ├┬┘
//╚═╝┴  └─┘┘└┘╚═╝╩═╝  ╚═╝└─┘┘└┘ ┴ ┴ ┴┴┘└┘└─┘┴└─
class OpenGL_container
{
  public:
    void init();
    void display();

  private:
    GLuint vao; //vertex array object
    GLuint vbo; //vertex buffer object

  //╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘
    GLuint main_display_shader;

    GLuint points_attrib;

  //╔╦╗┌─┐─┐ ┬┌┬┐┬ ┬┬─┐┌─┐┌─┐
  // ║ ├┤ ┌┴┬┘ │ │ │├┬┘├┤ └─┐
  // ╩ └─┘┴ └─ ┴ └─┘┴└─└─┘└─┘


};

#endif
