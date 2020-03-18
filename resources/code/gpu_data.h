#ifndef GPU_DATA
#define GPU_DATA

#include "includes.h"




//These are to keep track, since there's going to be quite a few
// referencing them with shaders[SHADER_NAME] when you want to use them

//  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
//  ╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
//  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘
typedef enum shader_index_t
{
  //what shaders do we need?
} shader_index;

std::vector<GLuint> shaders;


//  ╔╦╗┌─┐─┐ ┬┌┬┐┬ ┬┬─┐┌─┐┌─┐
//   ║ ├┤ ┌┴┬┘ │ │ │├┬┘├┤ └─┐
//   ╩ └─┘┴ └─ ┴ └─┘┴└─└─┘└─┘
typedef enum texture_index_t
{
  //what textures do we need?
} texture_index;

std::vector<GLuint> textures;



//  ╔═╗┌─┐┌─┐┌┐┌╔═╗╦    ╔═╗┌─┐┌┐┌┌┬┐┌─┐┬┌┐┌┌─┐┬─┐
//  ║ ║├─┘├┤ │││║ ╦║    ║  │ ││││ │ ├─┤││││├┤ ├┬┘
//  ╚═╝┴  └─┘┘└┘╚═╝╩═╝  ╚═╝└─┘┘└┘ ┴ ┴ ┴┴┘└┘└─┘┴└─
class OpenGL_container
{
public:
  void init();

private:



};
#endif
