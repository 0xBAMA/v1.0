#include "gpu_data.h"

void OpenGL_container::init()
{
  // glGenVertexArrays( 1, &vao );
  // glGenBuffers( 1, &vbo );
  // glBindVertexArray( vao );
  // glBindBuffer( GL_ARRAY_BUFFER, vbo );
  //
  // Shader s("resources/code/shaders/vertex.glsl", "resources/code/shaders/fragment.glsl");
  //
  // main_display_shader = s.Program;
  //
  // glBindAttribLocation( main_display_shader, 0, "i_color" );
  // glEnableVertexAttribArray( 0 );
  //
  // glBindAttribLocation( main_display_shader, 1, "i_position" );
  // glEnableVertexAttribArray( 1 );
  //
  // glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
  // glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * )(4 * sizeof(float)) );
  //
  // GLfloat width = 100.0;
  // GLfloat height = 100.0;
  //
  // const GLfloat g_vertex_buffer_data[] = {
  // /*  R, G, B, A, X, Y  */
  //     1, 0, 0, 1, 0, 0,
  //     0, 1, 0, 1, width, 0,
  //     0, 0, 1, 1, width, height,
  //
  //     1, 0, 0, 1, 0, 0,
  //     0, 0, 1, 1, width, height,
  //     1, 1, 1, 1, 0, height
  // };
  //
  // glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );
}

void OpenGL_container::display()
{
  // glBindVertexArray( vao );
  // glDrawArrays( GL_TRIANGLES, 0, 6 );
}
