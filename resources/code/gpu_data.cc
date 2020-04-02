#include "gpu_data.h"
#include "includes.h"

void OpenGL_container::init()
{

  cout << "  compiling main display shaders.........";
  Shader s("resources/code/shaders/main.vs.glsl", "resources/code/shaders/main.fs.glsl");

  main_display_shader = s.Program;
  cout << "done." << endl;

  SDL_Delay(30);

  //╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //║  │ ││││├─┘│ │ │ ├┤   ╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘

  cout << "  compiling sphere compute shader........";
  CShader cssphere("resources/code/shaders/sphere.cs.glsl");
  sphere_compute = cssphere.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling aabb compute shader..........";
  CShader cssaabb("resources/code/shaders/aabb.cs.glsl");
  aabb_compute = cssaabb.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling cylinder compute shader......";
  CShader cscylinder("resources/code/shaders/cylinder.cs.glsl");
  cylinder_compute = cscylinder.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling tube compute shader..........";
  CShader cstube("resources/code/shaders/tube.cs.glsl");
  tube_compute = cstube.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling cuboid compute shader........";
  CShader cscuboid("resources/code/shaders/cuboid.cs.glsl");
  cuboid_compute = cscuboid.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling triangle compute shader......";
  CShader cstriangle("resources/code/shaders/triangle.cs.glsl");
  triangle_compute = cstriangle.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling ellipsoid compute shader.....";
  CShader csellipsoid("resources/code/shaders/ellipsoid.cs.glsl");
  ellipsoid_compute = csellipsoid.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling clear all compute shader.....";
  CShader csclear_all("resources/code/shaders/clear_all.cs.glsl");
  clear_all_compute = csclear_all.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling unmask all compute shader....";
  CShader csunmask_all("resources/code/shaders/unmask_all.cs.glsl");
  unmask_all_compute = csunmask_all.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling toggle mask compute shader...";
  CShader cstoggle_mask("resources/code/shaders/toggle_mask.cs.glsl");
  toggle_mask_compute = cstoggle_mask.Program;
  SDL_Delay(30);
  cout << "done." << endl;


  cout << "  compiling blur compute shader..........";
  CShader csblur("resources/code/shaders/blur.cs.glsl");
  blur_compute = csblur.Program;
  SDL_Delay(30);
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
  cout << "  setting up vao, vbo....................";
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  cout << "done." << endl;

  //buffer the data
  cout << "  buffering vertex data..................";
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), &points[0]);
  cout << "done." << endl;

  //set up attributes
  cout << "  setting up attributes..................";
  GLuint points_attrib = glGetAttribLocation(main_display_shader, "vPosition");
  glEnableVertexAttribArray(points_attrib);
  glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));
  cout << "done." << endl;

  scale = 5.0f;
  phi   = 0.0f;
  theta = 0.0f;

  generate_diamond_square();
}

void OpenGL_container::load_textures()
{
  PerlinNoise p;
  std::vector<unsigned char> data;
  std::vector<unsigned char> data2;
  std::vector<unsigned char> data3;
  std::vector<unsigned char> data4;
  unsigned char val;


  cout << "  loading textures......................." << std::flush;

  for(int x = 0; x < DIM; x++)
  {
    for(int y = 0; y < DIM; y++)
    {
      for(int z = 0; z < DIM; z++)
      {
        val = (unsigned char)(p.noise(x*0.014,y*0.04,z*0.014) * 255);

        //populate the 4 component texture with some values
        data.push_back(0);                     //red
        data.push_back(val);                  //green
        data.push_back(0);                   //blue
        data.push_back(1);                  //alpha


        data2.push_back(val);                   //red
        data2.push_back(0);                    //green
        data2.push_back(0);                   //blue
        data2.push_back(1);                  //alpha

        // if(val < 127)
        // {
        //   data3.push_back(255);               //populate the mask texture with some zero values, or 255 values
        //   data4.push_back(0);
        // }
        // else
        // {
        //   data3.push_back(0);
        //   data4.push_back(255);
        // }

        data3.push_back(0);
        data4.push_back(0);
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



  glGenTextures(2, &mask_textures[0]);

  glBindTexture(GL_TEXTURE_3D, mask_textures[0]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data3[0]);
  glBindImageTexture(2, mask_textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);

  glBindTexture(GL_TEXTURE_3D, mask_textures[1]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data4[0]);
  glBindImageTexture(3, mask_textures[1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);



  //these are going to be standard textures, read only, with mipmaps and filtering
  glGenTextures(1, &perlin_texture);
  //3d texture for perlin noise - DIM on a side

  glGenTextures(1, &heightmap_texture);
  //2d texture for representation of a heightmap (greyscale) - also, DIM on a side


//intialize these so we know which textures we're using
  location_of_previous = 0;
  location_of_current = 1;

  location_of_previous_mask = 2;
  location_of_current_mask = 3;

  cout << "done." << endl;
}

void OpenGL_container::swap_blocks()
{
  // this is to swap between the two block textures
  // we read from one, write to the other, then swap - but do it by the number of the texture unit, so no data actually moves
  //  just one uniform integer changes, and it gets sent anyways
  if(location_of_current == 1)
  {
    location_of_current = 0;
    location_of_previous = 1;

    location_of_current_mask = 2;
    location_of_previous_mask = 3;
  }
  else
  {
    location_of_previous = 0;
    location_of_current = 1;

    location_of_previous_mask = 2;
    location_of_current_mask = 3;
  }
}

void OpenGL_container::generate_diamond_square()
{
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::default_random_engine engine{seed};
  std::uniform_real_distribution<float> distribution{0, 1};

  constexpr auto size = 513;
  constexpr auto edge = size - 1;

  uint8_t map[size][size] = {{0}};
  map[0][0] = map[edge][0] = map[0][edge] = map[edge][edge] = 128;

  heightfield::diamond_square_no_wrap(
      size,
      // random
      [&engine, &distribution](float range)
      {
          return distribution(engine) * range;
      },
      // variance
      [](int level) -> float
      {
          return 64.0f * std::pow(0.5f, level);
      },
      // at
      [&map](int x, int y) -> uint8_t&
      {
          return map[y][x];
      }
  );

  //put this data in a byte array
  //send it to the GPU

  // // Output PGM (Netpbm greyscale image format)
  // std::cout << "P2 " << size << ' ' << size << " 255\n";
  //
  // for (auto& row : map)
  // {
  //     for (auto& cell : row)
  //     {
  //         std::cout << static_cast<int>(cell) << " ";
  //     }
  //     std::cout << std::endl;
  // }
}

void OpenGL_containergenerate_perlin()
{

}



//╔╦╗┬─┐┌─┐┬ ┬  ╔═╗┬ ┬┌┐┌┌─┐┌┬┐┬┌─┐┌┐┌┌─┐
// ║║├┬┘├─┤│││  ╠╣ │ │││││   │ ││ ││││└─┐
//═╩╝┴└─┴ ┴└┴┘  ╚  └─┘┘└┘└─┘ ┴ ┴└─┘┘└┘└─┘


void OpenGL_container::draw_sphere(glm::vec3 location, float radius, glm::vec4 color, bool draw, bool mask) //done
{
//╔═╗┌─┐┬ ┬┌─┐┬─┐┌─┐
//╚═╗├─┘├─┤├┤ ├┬┘├┤
//╚═╝┴  ┴ ┴└─┘┴└─└─┘
  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(sphere_compute);

  //send the sphere-specific values
  glUniform1i(glGetUniformLocation(sphere_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(sphere_compute, "draw"), draw);
  glUniform1fv(glGetUniformLocation(sphere_compute, "radius"), 1, &radius);
  glUniform3fv(glGetUniformLocation(sphere_compute, "location"), 1, glm::value_ptr(location));
  glUniform4fv(glGetUniformLocation(sphere_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(sphere_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_perlin_noise()
{
//╔═╗┌─┐┬─┐┬  ┬┌┐┌  ╔╗╔┌─┐┬┌─┐┌─┐
//╠═╝├┤ ├┬┘│  ││││  ║║║│ ││└─┐├┤
//╩  └─┘┴└─┴─┘┴┘└┘  ╝╚╝└─┘┴└─┘└─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(sphere_compute);


  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(sphere_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, float thickness, glm::vec4 color, bool draw, bool mask)  //done
{
//╔╦╗┬─┐┬┌─┐┌┐┌┌─┐┬  ┌─┐
// ║ ├┬┘│├─┤││││ ┬│  ├┤
// ╩ ┴└─┴┴ ┴┘└┘└─┘┴─┘└─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(triangle_compute);
  glUniform1i(glGetUniformLocation(triangle_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(triangle_compute, "draw"), draw);
  glUniform1fv(glGetUniformLocation(triangle_compute, "thickness"), 1, &thickness);
  glUniform3fv(glGetUniformLocation(triangle_compute, "point1"), 1, glm::value_ptr(point1));
  glUniform3fv(glGetUniformLocation(triangle_compute, "point2"), 1, glm::value_ptr(point2));
  glUniform3fv(glGetUniformLocation(triangle_compute, "point3"), 1, glm::value_ptr(point3));
  glUniform4fv(glGetUniformLocation(triangle_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(triangle_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(triangle_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(triangle_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(triangle_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_ellipsoid(glm::vec3 center, glm::vec3 radii, glm::vec3 rotation, glm::vec4 color, bool draw, bool mask) //done
{
//╔═╗┬  ┬  ┬┌─┐┌─┐┌─┐┬┌┬┐
//║╣ │  │  │├─┘└─┐│ ││ ││
//╚═╝┴─┘┴─┘┴┴  └─┘└─┘┴─┴┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(ellipsoid_compute);

  //send the sphere-specific values
  glUniform1i(glGetUniformLocation(ellipsoid_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(ellipsoid_compute, "draw"), draw);
  glUniform3fv(glGetUniformLocation(ellipsoid_compute, "radii"), 1, glm::value_ptr(radii));
  glUniform3fv(glGetUniformLocation(ellipsoid_compute, "rotation"), 1, glm::value_ptr(rotation));
  glUniform3fv(glGetUniformLocation(ellipsoid_compute, "center"), 1, glm::value_ptr(center));
  glUniform4fv(glGetUniformLocation(ellipsoid_compute, "color"), 1, glm::value_ptr(color));


  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(ellipsoid_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(ellipsoid_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(ellipsoid_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(ellipsoid_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_cylinder(glm::vec3 bvec, glm::vec3 tvec, float radius, glm::vec4 color, bool draw, bool mask) //done
{
//╔═╗┬ ┬┬  ┬┌┐┌┌┬┐┌─┐┬─┐
//║  └┬┘│  ││││ ││├┤ ├┬┘
//╚═╝ ┴ ┴─┘┴┘└┘─┴┘└─┘┴└─

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(cylinder_compute);

  //send uniform variables specific to the aabb
  glUniform1i(glGetUniformLocation(cylinder_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(cylinder_compute, "draw"), draw);
  glUniform1fv(glGetUniformLocation(cylinder_compute, "radius"), 1, &radius);
  glUniform3fv(glGetUniformLocation(cylinder_compute, "bvec"), 1, glm::value_ptr(bvec));
  glUniform3fv(glGetUniformLocation(cylinder_compute, "tvec"), 1, glm::value_ptr(tvec));
  glUniform4fv(glGetUniformLocation(cylinder_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(cylinder_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(cylinder_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(cylinder_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(cylinder_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_tube(glm::vec3 bvec, glm::vec3 tvec, float inner_radius, float outer_radius, glm::vec4 color, bool draw, bool mask) //done
{
//╔╦╗┬ ┬┌┐ ┌─┐
// ║ │ │├┴┐├┤
// ╩ └─┘└─┘└─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(tube_compute);

  //send uniform variables specific to the aabb
  glUniform1i(glGetUniformLocation(tube_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(tube_compute, "draw"), draw);
  glUniform1fv(glGetUniformLocation(tube_compute, "iradius"), 1, &inner_radius);
  glUniform1fv(glGetUniformLocation(tube_compute, "oradius"), 1, &outer_radius);
  glUniform3fv(glGetUniformLocation(tube_compute, "bvec"), 1, glm::value_ptr(bvec));
  glUniform3fv(glGetUniformLocation(tube_compute, "tvec"), 1, glm::value_ptr(tvec));
  glUniform4fv(glGetUniformLocation(tube_compute, "color"), 1, glm::value_ptr(color));


  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(tube_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(tube_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(tube_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(tube_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_cuboid(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 e, glm::vec3 f, glm::vec3 g, glm::vec3 h, glm::vec4 color, bool draw, bool mask) //done
{
//╔═╗┬ ┬┌┐ ┌─┐┬┌┬┐
//║  │ │├┴┐│ ││ ││
//╚═╝└─┘└─┘└─┘┴─┴┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(cuboid_compute);

  //uniform variables defining shape
  glUniform1i(glGetUniformLocation(cuboid_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(cuboid_compute, "draw"), draw);
  glUniform3fv(glGetUniformLocation(cuboid_compute, "a"), 1, glm::value_ptr(a));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "b"), 1, glm::value_ptr(b));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "c"), 1, glm::value_ptr(c));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "d"), 1, glm::value_ptr(d));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "e"), 1, glm::value_ptr(e));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "f"), 1, glm::value_ptr(f));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "g"), 1, glm::value_ptr(g));
  glUniform3fv(glGetUniformLocation(cuboid_compute, "h"), 1, glm::value_ptr(h));
  glUniform4fv(glGetUniformLocation(cuboid_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(cuboid_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(cuboid_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(cuboid_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(cuboid_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_aabb(glm::vec3 min, glm::vec3 max, glm::vec4 color, bool draw, bool mask) //done
{
//╔═╗╔═╗╔╗ ╔╗
//╠═╣╠═╣╠╩╗╠╩╗
//╩ ╩╩ ╩╚═╝╚═╝

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(aabb_compute);

  //send uniform variables specific to the aabb
  glUniform1i(glGetUniformLocation(aabb_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(aabb_compute, "draw"), draw);
  glUniform3fv(glGetUniformLocation(aabb_compute, "mins"), 1, glm::value_ptr(min));
  glUniform3fv(glGetUniformLocation(aabb_compute, "maxs"), 1, glm::value_ptr(max));
  glUniform4fv(glGetUniformLocation(aabb_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(aabb_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(aabb_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(aabb_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(aabb_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_heightmap()
{
//╦ ╦┌─┐┬┌─┐┬ ┬┌┬┐┌┬┐┌─┐┌─┐
//╠═╣├┤ ││ ┬├─┤ │ │││├─┤├─┘
//╩ ╩└─┘┴└─┘┴ ┴ ┴ ┴ ┴┴ ┴┴

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(sphere_compute);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(sphere_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(sphere_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(sphere_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}



void OpenGL_container::blur(int radius, bool touch_alpha, bool respect_mask)
{
//╔╗ ┬  ┬ ┬┬─┐
//╠╩╗│  │ │├┬┘
//╚═╝┴─┘└─┘┴└─

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use clear_all program
  glUseProgram(blur_compute);

  //send uniform variables specific to the clear_all function
  glUniform1i(glGetUniformLocation(blur_compute, "radius"), radius);
  glUniform1i(glGetUniformLocation(blur_compute, "respect_mask"), respect_mask);
  glUniform1i(glGetUniformLocation(blur_compute, "touch_alpha"), touch_alpha);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(blur_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(blur_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(blur_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(blur_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data
}

void OpenGL_container::clear_all(bool respect_mask) //done
{
//╔═╗┬  ┌─┐┌─┐┬─┐  ╔═╗┬  ┬
//║  │  ├┤ ├─┤├┬┘  ╠═╣│  │
//╚═╝┴─┘└─┘┴ ┴┴└─  ╩ ╩┴─┘┴─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use clear_all program
  glUseProgram(clear_all_compute);

  //send uniform variables specific to the clear_all function
  glUniform1i(glGetUniformLocation(clear_all_compute, "respect_mask"), respect_mask);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(clear_all_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(clear_all_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(clear_all_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(clear_all_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data
}

void OpenGL_container::unmask_all() //done
{
//╦ ╦┌┐┌┌┬┐┌─┐┌─┐┬┌─  ╔═╗┬  ┬
//║ ║││││││├─┤└─┐├┴┐  ╠═╣│  │
//╚═╝┘└┘┴ ┴┴ ┴└─┘┴ ┴  ╩ ╩┴─┘┴─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(unmask_all_compute);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(unmask_all_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(unmask_all_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(unmask_all_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(unmask_all_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data
}

void OpenGL_container::toggle_mask()  //done
{
//╔╦╗┌─┐┌─┐┌─┐┬  ┌─┐  ╔╦╗┌─┐┌─┐┬┌─
// ║ │ ││ ┬│ ┬│  ├┤   ║║║├─┤└─┐├┴┐
// ╩ └─┘└─┘└─┘┴─┘└─┘  ╩ ╩┴ ┴└─┘┴ ┴

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //use aabb program
  glUseProgram(toggle_mask_compute);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(toggle_mask_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(toggle_mask_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(toggle_mask_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(toggle_mask_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data
}

void OpenGL_container::mask_by_color()
{
//╔╦╗┌─┐┌─┐┬┌─  ┌┐ ┬ ┬  ╔═╗┌─┐┬  ┌─┐┬─┐
//║║║├─┤└─┐├┴┐  ├┴┐└┬┘  ║  │ ││  │ │├┬┘
//╩ ╩┴ ┴└─┘┴ ┴  └─┘ ┴   ╚═╝└─┘┴─┘└─┘┴└─

}

void OpenGL_container::compute_static_lighting()
{
//╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐  ╔═╗┌┬┐┌─┐┌┬┐┬┌─┐  ╦  ┬┌─┐┬ ┬┌┬┐┬┌┐┌┌─┐
//║  │ ││││├─┘│ │ │ ├┤   ╚═╗ │ ├─┤ │ ││    ║  ││ ┬├─┤ │ │││││ ┬
//╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘  ╚═╝ ┴ ┴ ┴ ┴ ┴└─┘  ╩═╝┴└─┘┴ ┴ ┴ ┴┘└┘└─┘

}

void OpenGL_container::compute_ambient_occlusion()
{
//╔═╗┌┬┐┌┐ ┬┌─┐┌┐┌┌┬┐  ╔═╗┌─┐┌─┐┬  ┬ ┬┌─┐┬┌─┐┌┐┌
//╠═╣│││├┴┐│├┤ │││ │   ║ ║│  │  │  │ │└─┐││ ││││
//╩ ╩┴ ┴└─┘┴└─┘┘└┘ ┴   ╚═╝└─┘└─┘┴─┘└─┘└─┘┴└─┘┘└┘

}

void OpenGL_container::game_of_life_update()
{
//╔═╗┌─┐┌┬┐┌─┐  ┌─┐┌─┐  ╦  ┬┌─┐┌─┐
//║ ╦├─┤│││├┤   │ │├┤   ║  │├┤ ├┤
//╚═╝┴ ┴┴ ┴└─┘  └─┘└    ╩═╝┴└  └─┘

}

void OpenGL_container::wireworld_update()
{
//╦ ╦┬┬─┐┌─┐╦ ╦┌─┐┬─┐┬  ┌┬┐
//║║║│├┬┘├┤ ║║║│ │├┬┘│   ││
//╚╩╝┴┴└─└─┘╚╩╝└─┘┴└─┴─┘─┴┘

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

  glUniform1iv(glGetUniformLocation(main_display_shader, "current"),      1, &location_of_current);

  glDrawArrays( GL_TRIANGLES, 0, 6 );
}
