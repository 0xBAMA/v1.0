#include "gpu_data.h"

void OpenGL_container::init()
{

  Shader s("resources/code/shaders/vertex.glsl", "resources/code/shaders/fragment.glsl");

  main_display_shader = s.Program;

  //
  // A---------------B
  // |          .    |
  // |       .       |
  // |    .          |
  // |               |
  // C---------------D
  //


  //diagonal runs from C to B

  //we are at 0 in z
  //A is     0, height
  //B is width, height
  //C is     0, 0
  //D is width, 0

  // ImGuiIO& io = ImGui::GetIO();

  // int width = (int)io.DisplaySize.x;
  // int height= (int)io.DisplaySize.y;

  std::vector<glm::vec3>    points;

  points.push_back(glm::vec3(-1, 1, 0));  //A
  points.push_back(glm::vec3(-1,-1, 0));  //C
  points.push_back(glm::vec3( 1, 1, 0));  //B

  points.push_back(glm::vec3( 1, 1, 0));  //B
  points.push_back(glm::vec3(-1,-1, 0));  //C
  points.push_back(glm::vec3( 1,-1, 0));  //D

  // points.push_back(glm::vec3( 0, height, 0));  //A
  // points.push_back(glm::vec3( 0, 0, 0));  //C
  // points.push_back(glm::vec3( width, height, 0));  //B
  //
  // points.push_back(glm::vec3( width, height, 0));  //B
  // points.push_back(glm::vec3( 0, 0, 0));  //C
  // points.push_back(glm::vec3( width, 0, 0));  //D

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );

  const GLuint num_bytes_points = sizeof(glm::vec3) * points.size();
  GLint num_bytes = num_bytes_points;

  glBufferData(GL_ARRAY_BUFFER, num_bytes, NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);

  cout << endl << endl << "setting up points attrib" << endl << std::flush;

  points_attrib = glGetAttribLocation(main_display_shader, "vPosition");
  glEnableVertexAttribArray(points_attrib);
  glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));

}

void OpenGL_container::display()
{
  glBindVertexArray( vao );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glEnableVertexAttribArray(points_attrib);

  glUseProgram(main_display_shader);

  // GLint viewport[4];
  // glGetIntegerv( GL_VIEWPORT, viewport );
  // cout << "viewport is: " << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << endl;

  glDrawArrays( GL_TRIANGLES, 0, 6 );
}
