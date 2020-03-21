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
    void load_textures();

    void display();

  private:
    GLuint vao; //vertex array object
    GLuint vbo; //vertex buffer object

    std::vector<glm::vec3> points;

    float scale;
    float phi;
    float theta;

  //╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘
    GLuint main_display_shader;


  //╔╦╗┌─┐─┐ ┬┌┬┐┬ ┬┬─┐┌─┐┌─┐
  // ║ ├┤ ┌┴┬┘ │ │ │├┬┘├┤ └─┐
  // ╩ └─┘┴ └─ ┴ └─┘┴└─└─┘└─┘


};

#endif
