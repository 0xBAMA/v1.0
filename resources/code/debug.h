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

  bool show_notification_severity = true;
  if(severity == GL_DEBUG_SEVERITY_NOTIFICATION && show_notification_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_NOTIFICATION, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, message );
}

void gl_debug_enable()
{
  //DEBUG ENABLE
  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );      //getting a seg fault here, I think
  cout << "GL debug output enabled." << endl;
}
