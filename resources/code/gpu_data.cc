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
  cout << "  compiling main display shaders...";
  Shader s("resources/code/shaders/main.vs.glsl", "resources/code/shaders/main.fs.glsl");

  main_display_shader = s.Program;
  cout << "done." << endl;


  //╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //║  │ ││││├─┘│ │ │ ├┤   ╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘

  cout << "  compiling sphere compute shader...";
  CShader cssphere("resources/code/shaders/sphere.cs.glsl");

  sphere_compute = cssphere.Program;
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

  scale = 5.0f;
  phi   = 0.0f;
  theta = 0.0f;
}

void OpenGL_container::load_textures()
{
  PerlinNoise p;
  std::vector<unsigned char> data;
  std::vector<unsigned char> data2;
  std::vector<unsigned char> data3;
  unsigned char val;

  #define DIM 256
  // #define DIM 512


  for(int x = 0; x < DIM; x++)
  {
    for(int y = 0; y < DIM; y++)
    {
      for(int z = 0; z < DIM; z++)
      {
        val = (unsigned char)(p.noise(x*0.014,y*0.04,z*0.014) * 255);

        //populate the 4 component texture with some values
        data.push_back(val);                     //red
        data.push_back(val);                    //green
        data.push_back(val);                   //blue

        if(val > 100)
          data.push_back(0);                  //alpha
        else if(val > 75)
          data.push_back(10);
        else if(val > 50)
          data.push_back(30);
        else
          data.push_back(255);



        if((x*y*z)%5==0)
        {
          data2.push_back(val);                     //red
          data2.push_back(val);                    //green
          data2.push_back( 0);                    //blue
          data2.push_back( 10);
        }
        else
        {
          data2.push_back(255);                     //red
          data2.push_back(val);                    //green
          data2.push_back( 0);                    //blue
          data2.push_back( 1);
        }


        data3.push_back(0);               //populate the mask texture with some zero values
      }
    }
  }

  glGenTextures(2, &block_textures[0]);

  glBindTexture(GL_TEXTURE_3D, block_textures[0]); // use the specified ID
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
  glBindImageTexture(0, block_textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

  glBindTexture(GL_TEXTURE_3D, block_textures[1]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &data2[0]);
  glBindImageTexture(1, block_textures[1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);


  location_of_current = 0;


  glGenTextures(2, &mask_textures[0]);

  glBindTexture(GL_TEXTURE_3D, mask_textures[0]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data3[0]);
  glBindImageTexture(2, mask_textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);

  glBindTexture(GL_TEXTURE_3D, mask_textures[1]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data3[0]);
  glBindImageTexture(3, mask_textures[1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);


  //these are going to be standard textures, read only, with mipmaps and filtering
  glGenTextures(1, &perlin_texture);
  glGenTextures(1, &heightmap_texture);


  cout << "finished load_textures()" << endl << endl;

  //testing compute shader

  glUseProgram(sphere_compute);
  glUniform1iv(glGetUniformLocation(sphere_compute, "current"), 1, &location_of_current);


  glDispatchCompute( DIM/8, DIM/8, DIM/8 );

  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

}

void OpenGL_container::display()
{
  glUseProgram(main_display_shader);

  ImGuiIO& io = ImGui::GetIO();
  GLint xres = (int)io.DisplaySize.x;
  GLint yres = (int)io.DisplaySize.y;

  // float f = SDL_GetTicks(); // this is how you get the time elapsed, in ms, if you want to do animated stuff

  glUniform1iv(glGetUniformLocation(main_display_shader, "x_resolution"), 1, &xres);
  glUniform1iv(glGetUniformLocation(main_display_shader, "y_resolution"), 1, &yres);
  glUniform1fv(glGetUniformLocation(main_display_shader, "scale"),        1, &scale);
  glUniform1fv(glGetUniformLocation(main_display_shader, "uphi"),         1, &phi);
  glUniform1fv(glGetUniformLocation(main_display_shader, "utheta"),       1, &theta);

  glUniform4fv(glGetUniformLocation(main_display_shader, "clear_color"), 1, glm::value_ptr(clear_color));


  // this is to swap between the two block textures
  // we read from one, write to the other, then swap - but do it by the number of the texture unit, so no data actually moves
  //  just one uniform integer changes, and it gets sent anyways
  if(location_of_current == 1)
    location_of_current = 0;
  else
    location_of_current = 1;

  glUniform1iv(glGetUniformLocation(main_display_shader, "current"),      1, &location_of_current);

  glDrawArrays( GL_TRIANGLES, 0, 6 );
}
