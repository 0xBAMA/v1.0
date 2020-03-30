#ifndef DEBUG
#define DEBUG

#include "includes.h"

//gl debug dump
void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  bool show_high_severity         = true;
  if(severity == GL_DEBUG_SEVERITY_HIGH && show_high_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_HIGH, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, message );

  bool show_medium_severity       = true;
  if(severity == GL_DEBUG_SEVERITY_MEDIUM && show_medium_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_MEDIUM, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, message );

  bool show_low_severity          = true;
  if(severity == GL_DEBUG_SEVERITY_LOW && show_low_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_LOW, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, message );

  bool show_notification_severity = false;
  if(severity == GL_DEBUG_SEVERITY_NOTIFICATION && show_notification_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_NOTIFICATION, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, message );
}

void gl_debug_enable()
{
  //DEBUG ENABLE
  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );      //getting a seg fault here, I think
  cout << "OpenGL debug output enabled." << endl << endl;


  //report all gl extensions - useful on different platforms

  // GLint n;
  //
  // glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  // cout << "starting dump of " << n << " extensions" << endl;
  //
  // for(int i=0; i<n; i++)
  //   cout << i << ": " << glGetStringi(GL_EXTENSIONS, i) << endl;
  //
  // cout << endl;




  //gl info re:texture size, texture units

  // GLint val;
  // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
  // cout << "max texture size reports: " << val << endl << endl;
  //
  // glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &val);
  // cout << "max 3dtexture size reports: " << val << " on all 3 edges" << endl << endl;
  //
  // glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &val);
  // cout << "max compute texture image units reports: " << val << endl << endl;
}

#endif
