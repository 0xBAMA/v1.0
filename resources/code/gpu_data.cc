#include "gpu_data.h"

void OpenGL_container::init()
{
  GLint val;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
  cout << "max texture size reports: " << val << endl << endl;

  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &val);
  cout << "max 3dtexture size reports: " << val << " on all 3 edges" << endl << endl;

  glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &val);
  cout << "max compute texture image units reports: " << val << endl << endl;


  cout << "starting init" << endl;
  cout << "  compiling shader...";
  Shader s("resources/code/shaders/main.vs.glsl", "resources/code/shaders/main.fs.glsl");

  main_display_shader = s.Program;
  cout << "done." << endl;

  // A---------------B
  // |          .    |
  // |       .       |
  // |    .          |
  // |               |
  // C---------------D

  //diagonal runs from C to B
  //A is -1, 1
  //B is  1, 1
  //C is -1,-1
  //D is  1,-1

  points.clear();
  points.push_back(glm::vec3(-1, 1, 0));  //A
  points.push_back(glm::vec3(-1,-1, 0));  //C
  points.push_back(glm::vec3( 1, 1, 0));  //B

  points.push_back(glm::vec3( 1, 1, 0));  //B
  points.push_back(glm::vec3(-1,-1, 0));  //C
  points.push_back(glm::vec3( 1,-1, 0));  //D

  //vao, vbo
  cout << "  setting up vao, vbo...";
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  cout << "done." << endl;

  //buffer the data
  cout << "  buffering data...";
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), &points[0]);
  cout << "done." << endl;

  //set up attributes
  cout << "  setting up attributes...";
  GLuint points_attrib = glGetAttribLocation(main_display_shader, "vPosition");
  glEnableVertexAttribArray(points_attrib);
  glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));
  cout << "done." << endl << endl;

}

void OpenGL_container::load_textures()
{

}

void OpenGL_container::display()
{
  glUseProgram(main_display_shader);

  ImGuiIO& io = ImGui::GetIO();

  GLint xres = (int)io.DisplaySize.x;
  glUniform1iv(glGetUniformLocation(main_display_shader, "x_resolution"), 1, &xres);

  GLint yres = (int)io.DisplaySize.y;
  glUniform1iv(glGetUniformLocation(main_display_shader, "y_resolution"), 1, &yres);

  glDrawArrays( GL_TRIANGLES, 0, 6 );
}
