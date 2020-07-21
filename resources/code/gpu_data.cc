#include "gpu_data.h"
#include "includes.h"

void OpenGL_container::init()
{

  cout << "  compiling main display shaders...............";
  Shader s("resources/code/shaders/main.vs.glsl", "resources/code/shaders/main.fs.glsl");

  main_display_shader = s.Program;
  cout << "done." << endl;

  //╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //║  │ ││││├─┘│ │ │ ├┤   ╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘

  cout << "  compiling sphere compute shader..............";
  CShader cssphere("resources/code/shaders/sphere.cs.glsl");
  sphere_compute = cssphere.Program;
  cout << "done." << endl;


  cout << "  compiling aabb compute shader................";
  CShader cssaabb("resources/code/shaders/aabb.cs.glsl");
  aabb_compute = cssaabb.Program;
  cout << "done." << endl;


  cout << "  compiling cylinder compute shader............";
  CShader cscylinder("resources/code/shaders/cylinder.cs.glsl");
  cylinder_compute = cscylinder.Program;
  cout << "done." << endl;


  cout << "  compiling tube compute shader................";
  CShader cstube("resources/code/shaders/tube.cs.glsl");
  tube_compute = cstube.Program;
  cout << "done." << endl;


  cout << "  compiling cuboid compute shader..............";
  CShader cscuboid("resources/code/shaders/cuboid.cs.glsl");
  cuboid_compute = cscuboid.Program;
  cout << "done." << endl;


  cout << "  compiling grid compute shader................";
  CShader csgrid("resources/code/shaders/grid.cs.glsl");
  grid_compute = csgrid.Program;
  cout << "done." << endl;


  cout << "  compiling triangle compute shader............";
  CShader cstriangle("resources/code/shaders/triangle.cs.glsl");
  triangle_compute = cstriangle.Program;
  cout << "done." << endl;


  cout << "  compiling ellipsoid compute shader...........";
  CShader csellipsoid("resources/code/shaders/ellipsoid.cs.glsl");
  ellipsoid_compute = csellipsoid.Program;
  cout << "done." << endl;


  cout << "  compiling clear all compute shader...........";
  CShader csclear_all("resources/code/shaders/clear_all.cs.glsl");
  clear_all_compute = csclear_all.Program;
  cout << "done." << endl;


  cout << "  compiling unmask all compute shader..........";
  CShader csunmask_all("resources/code/shaders/unmask_all.cs.glsl");
  unmask_all_compute = csunmask_all.Program;
  cout << "done." << endl;


  cout << "  compiling toggle mask compute shader.........";
  CShader cstoggle_mask("resources/code/shaders/toggle_mask.cs.glsl");
  toggle_mask_compute = cstoggle_mask.Program;
  cout << "done." << endl;


  cout << "  compiling blur compute shader................";
  CShader csblur("resources/code/shaders/blur.cs.glsl");
  blur_compute = csblur.Program;
  cout << "done." << endl;


  cout << "  compiling shift compute shader...............";
  CShader csshift("resources/code/shaders/shift.cs.glsl");
  shift_compute = csshift.Program;
  cout << "done." << endl;


  cout << "  compiling heightmap compute shader...........";
  CShader csheightmap("resources/code/shaders/heightmap.cs.glsl");
  heightmap_compute = csheightmap.Program;
  cout << "done." << endl;


  cout << "  compiling perlin noise compute shader........";
  CShader csperlin("resources/code/shaders/perlin.cs.glsl");
  perlin_noise_compute = csperlin.Program;
  cout << "done." << endl;


  cout << "  compiling mask by color compute shader.......";
  CShader csmaskbycolor("resources/code/shaders/mask_by_color.cs.glsl");
  mask_by_color_compute = csmaskbycolor.Program;
  cout << "done." << endl;


  cout << "  compiling AO compute shader..................";
  CShader csao("resources/code/shaders/ambient_occlusion.cs.glsl");
  ambient_occlusion_compute = csao.Program;
  cout << "done." << endl;


  cout << "  compiling mash compute shader................";
  CShader csmash("resources/code/shaders/mash.cs.glsl");
  mash_compute = csmash.Program;
  cout << "done." << endl;


  cout << "  compiling static lighting compute shader(s)..";
  //there's two, one to clear the block, the same structure as all the others
  CShader cslightingclear("resources/code/shaders/light_clear.cs.glsl");
  lighting_clear_compute = cslightingclear.Program;


  //and one to do the actual raycasting operation, in the same style as the rendering operation
  CShader csstaticlighting("resources/code/shaders/directional.cs.glsl");
  static_lighting_compute = csstaticlighting.Program;
  cout << "done." << endl;

 
  cout << "  compiling fake global illumination shader....";
  CShader csfakeGI("resources/code/shaders/fakeGI.cs.glsl");
  fakeGI_compute = csfakeGI.Program;
  cout << "done." << endl;

//  cout << "  compiling Game of Life compute shader........";
//  CShader csgameoflife("resources/code/shaders/gol.cs.glsl");
//  game_of_life_update_compute = csgameoflife.Program;
//  cout << "done." << endl;


//  cout << "  compiling WireWorld3d compute shader.........";
//  CShader cswireworld("resources/code/shaders/wireworld.cs.glsl");
//  wireworld_update_compute = cswireworld.Program;
//  cout << "done." << endl;






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
  cout << "  setting up vao, vbo..........................";
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  cout << "done." << endl;

  //buffer the data
  cout << "  buffering vertex data........................";
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), &points[0]);
  cout << "done." << endl;

  //set up attributes
  cout << "  setting up attributes........................";
  GLuint points_attrib = glGetAttribLocation(main_display_shader, "vPosition");
  glEnableVertexAttribArray(points_attrib);
  glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));
  cout << "done." << endl;

  //rotation, scaling of view
  scale = 5.0f;
  theta = 0.0f;
  phi   = 0.0f;
}

void OpenGL_container::load_textures()
{
  //PerlinNoise p;
  std::vector<unsigned char> data;  //back buffer, not seen
  std::vector<unsigned char> data2; //XOR texture as the starting value for color
  std::vector<unsigned char> data3; //initial mask values
  std::vector<unsigned char> data4; //initial light buffer values
  //unsigned char val;


  cout << "  loading textures............................." << std::flush;

  for(int x = 0; x < DIM; x++)
  {
    for(int y = 0; y < DIM; y++)
    {
      for(int z = 0; z < DIM; z++)
      {
        //val = (unsigned char)(p.noise(x*0.014,y*0.04,z*0.014) * 255);

        //populate the 4 component texture with some values
        data.push_back(0);                     //red
        data.push_back(0);                  //green
        data.push_back(0);                   //blue
        data.push_back(1);                  //alpha


        // data2.push_back(val);                   //red
        // data2.push_back(0);                    //green
        // data2.push_back(0);                   //blue
        // data2.push_back(1);                  //alpha


        data2.push_back(((unsigned char)(x%256) ^ (unsigned char)(y%256) ^ (unsigned char)(z%256)));
        data2.push_back(((unsigned char)(x%256) ^ (unsigned char)(y%256) ^ (unsigned char)(z%256)));
        data2.push_back(((unsigned char)(x%256) ^ (unsigned char)(y%256) ^ (unsigned char)(z%256)));
        // data2.push_back(((unsigned char)(x%256) ^ (unsigned char)(y%256) ^ (unsigned char)(z%256)));
        data2.push_back(255);



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
        data4.push_back(64); //127 maps to 0.5, in the shader 
        //data4.push_back((x*y*z)%256); //interesting pattern
      }
    }
  }

  glGenTextures(2, &block_textures[0]);

  glActiveTexture(GL_TEXTURE0+0);
  glBindTexture(GL_TEXTURE_3D, block_textures[0]); // use the specified ID
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
  glBindImageTexture(0, block_textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);






  glActiveTexture(GL_TEXTURE0+1); 
  glBindTexture(GL_TEXTURE_3D, block_textures[1]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &data2[0]);
  glBindImageTexture(1, block_textures[1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);





  glGenTextures(2, &mask_textures[0]);

  glActiveTexture(GL_TEXTURE0+2);
  glBindTexture(GL_TEXTURE_3D, mask_textures[0]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data3[0]);
  glBindImageTexture(2, mask_textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);

  glActiveTexture(GL_TEXTURE0+3);
  glBindTexture(GL_TEXTURE_3D, mask_textures[1]);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0,  GL_RED, GL_UNSIGNED_BYTE, &data3[0]);
  glBindImageTexture(3, mask_textures[1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);





  glGenTextures(1, &light_texture);
  
  glActiveTexture(GL_TEXTURE0+4);
  glBindTexture(GL_TEXTURE_3D, light_texture);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, DIM, DIM, DIM, 0, GL_RED, GL_UNSIGNED_BYTE, &data4[0]);
  glBindImageTexture(4, light_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);



  //these are going to be standard textures, read only, with mipmaps and filtering
  glGenTextures(1, &perlin_texture);
  glActiveTexture(GL_TEXTURE0+5);
  glBindTexture(GL_TEXTURE_3D, perlin_texture);

  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
  //3d texture for perlin noise - DIM on a side
  generate_perlin_noise(0.014, 0.04, 0.014);


  glGenTextures(1, &heightmap_texture);
  glActiveTexture(GL_TEXTURE0+6);  
  glBindTexture(GL_TEXTURE_2D, heightmap_texture);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  //2d texture for representation of a heightmap (greyscale) - also, DIM on a side
  generate_heightmap_diamond_square();

//intialize these so we know which textures we're using
  location_of_previous = 0;
  location_of_current = 1;

  location_of_previous_mask = 2;
  location_of_current_mask = 3;

  location_of_light_buffer = 4;

  location_of_perlin_noise = 5;
  location_of_heightmap = 6;

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

void OpenGL_container::generate_heightmap_diamond_square()
{
  long unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::default_random_engine engine{seed};
  std::uniform_real_distribution<float> distribution{0, 1};

  constexpr auto size =  DIM + 1;
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


  std::vector<unsigned char> data;


  for(int x = 0; x < DIM; x++)
  {
    for(int y = 0; y < DIM; y++)
  	{
      data.push_back(map[x][y]);
      data.push_back(map[x][y]);
      data.push_back(map[x][y]);
      data.push_back(255);
  	}
  }


  //send it to the GPU
    glBindTexture(GL_TEXTURE_2D, heightmap_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DIM, DIM, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);


}

void OpenGL_container::generate_heightmap_perlin()
{
  std::vector<unsigned char> data;

  PerlinNoise p;

    float xscale = 0.014f;
    float yscale = 0.04f;

    //might add more parameters at some point, but we'll see

  //create the byte array - parameters?
  //send it to the gpu

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-500.0,500.0);

//    float offset = distribution(generator);


    static float offset = 0;

    for(int x = 0; x < DIM; x++)
    {
        for(int y = 0; y < DIM; y++)
        {
            data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,offset) * 255));
            data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,offset) * 255));
            data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,offset) * 255));
            data.push_back(255);
        }
    }

    offset += 0.5;


    glBindTexture(GL_TEXTURE_2D, heightmap_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DIM, DIM, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL_container::generate_heightmap_XOR()
{
  //create the byte array
  //send it to the gpu


  std::vector<unsigned char> data;

  for(int x = 0; x < DIM; x++)
  {
    for(int y = 0; y < DIM; y++)
    {
      //cout << " "<< ((unsigned char)(x%256) ^ (unsigned char)(y%256));
      data.push_back((unsigned char)(x%256) ^ (unsigned char)(y%256));
      data.push_back((unsigned char)(x%256) ^ (unsigned char)(y%256));
      data.push_back((unsigned char)(x%256) ^ (unsigned char)(y%256));
      data.push_back(255);
    }
  }

  //send the data to the gpu

    glBindTexture(GL_TEXTURE_2D, heightmap_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DIM, DIM, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL_container::generate_perlin_noise(float xscale=0.014, float yscale=0.04, float zscale=0.014)
{
    PerlinNoise p;
    std::vector<unsigned char> data;

    for(int x = 0; x < DIM; x++)
        for(int y = 0; y < DIM; y++)
            for(int z = 0; z < DIM; z++)
            {
                data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,z*zscale) * 255));
                data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,z*zscale) * 255));
                data.push_back((unsigned char)(p.noise(x*xscale,y*yscale,z*zscale) * 255));
                data.push_back(255);
            }


   glBindTexture(GL_TEXTURE_3D, perlin_texture);
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
   glGenerateMipmap(GL_TEXTURE_3D);
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

void OpenGL_container::draw_perlin_noise(float low_thresh, float high_thresh, bool smooth, glm::vec4 color, bool draw, bool mask)   //done
{
//╔═╗┌─┐┬─┐┬  ┬┌┐┌  ╔╗╔┌─┐┬┌─┐┌─┐
//╠═╝├┤ ├┬┘│  ││││  ║║║│ ││└─┐├┤
//╩  └─┘┴└─┴─┘┴┘└┘  ╝╚╝└─┘┴└─┘└─┘

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(perlin_noise_compute);

  glUniform1i(glGetUniformLocation(perlin_noise_compute, "usmooth"), smooth);
  glUniform1i(glGetUniformLocation(perlin_noise_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(perlin_noise_compute, "draw"), draw);
  glUniform1i(glGetUniformLocation(perlin_noise_compute, "tex"), location_of_perlin_noise);
  glUniform1f(glGetUniformLocation(perlin_noise_compute, "low_thresh"), low_thresh);
  glUniform1f(glGetUniformLocation(perlin_noise_compute, "high_thresh"), high_thresh);
  glUniform4fv(glGetUniformLocation(perlin_noise_compute, "ucolor"), 1, glm::value_ptr(color));


  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(perlin_noise_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(perlin_noise_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(perlin_noise_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(perlin_noise_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}

void OpenGL_container::draw_grid(glm::ivec3 spacing, glm::ivec3 widths, glm::ivec3 offsets, glm::vec4 color, bool draw, bool mask)
{
//  ╔═╗┬─┐┬┌┬┐
//  ║ ╦├┬┘│ ││
//  ╚═╝┴└─┴─┴┘

  swap_blocks();

  glUseProgram(grid_compute);
  glUniform1i(glGetUniformLocation(grid_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(grid_compute, "draw"), draw);
  
  glUniform3i(glGetUniformLocation(grid_compute, "spacing"), spacing.x, spacing.y, spacing.z);
  glUniform3i(glGetUniformLocation(grid_compute, "offsets"), offsets.x, offsets.y, offsets.z);
  glUniform3i(glGetUniformLocation(grid_compute, "width"), widths.x, widths.y, widths.z);

  glUniform4fv(glGetUniformLocation(grid_compute, "color"), 1, glm::value_ptr(color));

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(grid_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(grid_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(grid_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(triangle_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

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

void OpenGL_container::draw_heightmap(float height_scale, bool height_color, glm::vec4 color, bool mask, bool draw) //done
{
//╦ ╦┌─┐┬┌─┐┬ ┬┌┬┐┌┬┐┌─┐┌─┐
//╠═╣├┤ ││ ┬├─┤ │ │││├─┤├─┘
//╩ ╩└─┘┴└─┘┴ ┴ ┴ ┴ ┴┴ ┴┴

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();

  //testing compute shader
  glUseProgram(heightmap_compute);


  //sampler is in unit 6, so that can be sent directly
  //vscale scales the map vertically
  //height color tells whether to vary the color with the height, or false to have solid color
  //draw and mask are the same as before

  glUniform1i(glGetUniformLocation(heightmap_compute, "mask"), mask);
  glUniform1i(glGetUniformLocation(heightmap_compute, "draw"), draw);
  glUniform1i(glGetUniformLocation(heightmap_compute, "height_color"), height_color);
  glUniform1i(glGetUniformLocation(heightmap_compute, "map"), location_of_heightmap);
  glUniform1f(glGetUniformLocation(heightmap_compute, "vscale"), height_scale);
  glUniform4fv(glGetUniformLocation(heightmap_compute, "color"), 1, glm::value_ptr(color));



  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(heightmap_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(heightmap_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(heightmap_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(heightmap_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data

}



void OpenGL_container::blur(int radius, bool touch_alpha, bool respect_mask)  //done
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

void OpenGL_container::mask_by_color(bool r, bool g, bool b, bool a, bool l, glm::vec4 color, float l_val, float r_var, float g_var, float b_var, float a_var, float l_var)
{
//╔╦╗┌─┐┌─┐┬┌─  ┌┐ ┬ ┬  ╔═╗┌─┐┬  ┌─┐┬─┐
//║║║├─┤└─┐├┴┐  ├┴┐└┬┘  ║  │ ││  │ │├┬┘
//╩ ╩┴ ┴└─┘┴ ┴  └─┘ ┴   ╚═╝└─┘┴─┘└─┘┴└─

  //"current" values become "previous" values, "previous" values will become "current" values, as they will be overwritten with new data
  swap_blocks();  

  glUseProgram(mask_by_color_compute);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current_mask"), 1, &location_of_current_mask);

  //send the lighting texture handle
  glUniform1iv(glGetUniformLocation(mask_by_color_compute, "lighting"), 1, &location_of_light_buffer);
  
  glUniform1i(glGetUniformLocation(mask_by_color_compute, "use_r"), r);
  glUniform1i(glGetUniformLocation(mask_by_color_compute, "use_g"), g);
  glUniform1i(glGetUniformLocation(mask_by_color_compute, "use_b"), b);
  glUniform1i(glGetUniformLocation(mask_by_color_compute, "use_a"), a);

  glUniform1i(glGetUniformLocation(mask_by_color_compute, "use_l"), l);


  glUniform4fv(glGetUniformLocation(mask_by_color_compute, "color"), 1, glm::value_ptr(color));
  glUniform1f(glGetUniformLocation(mask_by_color_compute, "l_val"), l_val);


  glUniform1f(glGetUniformLocation(mask_by_color_compute, "r_var"), r_var);
  glUniform1f(glGetUniformLocation(mask_by_color_compute, "g_var"), g_var);
  glUniform1f(glGetUniformLocation(mask_by_color_compute, "b_var"), b_var);
  glUniform1f(glGetUniformLocation(mask_by_color_compute, "a_var"), a_var);

  glUniform1f(glGetUniformLocation(mask_by_color_compute, "l_var"), l_var);


  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

  //postcondition - "current" values have the most up-to-date data
}


void OpenGL_container::shift(glm::ivec3 movement, bool loop, int mode)
{
//  ╔═╗┬ ┬┬┌─┐┌┬┐
//  ╚═╗├─┤│├┤  │ 
//  ╚═╝┴ ┴┴└   ┴ 
  swap_blocks();  

  glUseProgram(shift_compute);

  //send movement, loop, mode
  glUniform1i(glGetUniformLocation(shift_compute, "loop"), loop);
  glUniform1i(glGetUniformLocation(shift_compute, "mode"),  mode);
  glUniform3i(glGetUniformLocation(shift_compute, "movement"), movement.x, movement.y, movement.z);

  //send the preveious texture handles
  glUniform1iv(glGetUniformLocation(shift_compute, "previous"), 1, &location_of_previous);
  glUniform1iv(glGetUniformLocation(shift_compute, "previous_mask"), 1, &location_of_previous_mask);

  //send the current texture handles
  glUniform1iv(glGetUniformLocation(shift_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(shift_compute, "current_mask"), 1, &location_of_current_mask);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 ); //workgroup is 8x8x8, so divide each dimension by 8

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

}


void OpenGL_container::lighting_clear(float intensity)
{
  glUseProgram(lighting_clear_compute);

  glUniform1f(glGetUniformLocation(lighting_clear_compute, "intensity"), intensity);

  glUniform1iv(glGetUniformLocation(lighting_clear_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(lighting_clear_compute, "lighting"), 1, &location_of_light_buffer);

  //dispatch the job
  glDispatchCompute( DIM/8, DIM/8, DIM/8 );

  //wait for things to synchronize
  glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

}


void OpenGL_container::compute_static_lighting(float theta, float phi, float initial_ray_intensity)
{
//╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐  ╔═╗┌┬┐┌─┐┌┬┐┬┌─┐  ╦  ┬┌─┐┬ ┬┌┬┐┬┌┐┌┌─┐
//║  │ ││││├─┘│ │ │ ├┤   ╚═╗ │ ├─┤ │ ││    ║  ││ ┬├─┤ │ │││││ ┬ 
//╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘  ╚═╝ ┴ ┴ ┴ ┴ ┴└─┘  ╩═╝┴└─┘┴ ┴ ┴ ┴┘└┘└─┘
    
    //this will only manipulate the light buffer - it doesn't need handles for the previous or current mask/color images

    glUseProgram(static_lighting_compute);

    glUniform1f(glGetUniformLocation(static_lighting_compute, "utheta"), theta);
    glUniform1f(glGetUniformLocation(static_lighting_compute, "uphi"), phi);
    glUniform1f(glGetUniformLocation(static_lighting_compute, "light_dim"), LIGHT_DIM);
    glUniform1f(glGetUniformLocation(static_lighting_compute, "light_intensity"), initial_ray_intensity);

    glUniform1iv(glGetUniformLocation(static_lighting_compute, "current"), 1, &location_of_current);
    glUniform1iv(glGetUniformLocation(static_lighting_compute, "lighting"), 1, &location_of_light_buffer);

    glDispatchCompute( LIGHT_DIM/8, LIGHT_DIM/8, 1 ); //workgroup is 8x8x1, so divide x and y by 8

    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
}

void OpenGL_container::compute_ambient_occlusion(int radius)
{
//╔═╗┌┬┐┌┐ ┬┌─┐┌┐┌┌┬┐  ╔═╗┌─┐┌─┐┬  ┬ ┬┌─┐┬┌─┐┌┐┌
//╠═╣│││├┴┐│├┤ │││ │   ║ ║│  │  │  │ │└─┐││ ││││
//╩ ╩┴ ┴└─┘┴└─┘┘└┘ ┴   ╚═╝└─┘└─┘┴─┘└─┘└─┘┴└─┘┘└┘
    glUseProgram(ambient_occlusion_compute);
    //this one just directly manipulates the color data

    glUniform1i(glGetUniformLocation(ambient_occlusion_compute, "radius"), radius);
    
    glUniform1iv(glGetUniformLocation(ambient_occlusion_compute, "current"), 1, &location_of_current);
    glUniform1iv(glGetUniformLocation(ambient_occlusion_compute, "lighting"), 1, &location_of_light_buffer);

    glDispatchCompute(DIM/8, DIM/8, DIM/8);

    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
}

void OpenGL_container::compute_fake_GI(float factor, float sky_intensity, float thresh)
{
  glUseProgram(fakeGI_compute);

  glUniform1iv(glGetUniformLocation(fakeGI_compute, "current"), 1, &location_of_current);
  glUniform1iv(glGetUniformLocation(fakeGI_compute, "lighting"), 1, &location_of_light_buffer);

  glUniform1f(glGetUniformLocation(fakeGI_compute, "scale_factor"), factor);
  glUniform1f(glGetUniformLocation(fakeGI_compute, "alpha_thresh"), thresh);
  glUniform1f(glGetUniformLocation(fakeGI_compute, "sky_intensity"), sky_intensity);

  // This has a sequential dependence - from the same guy who did the Voxel Automata Terrain, Brent Werness:
  //   "Totally faked the GI!  It just casts out 9 rays in upwards facing the lattice directions.
  //    If it escapes it gets light from the sky, otherwise it gets some fraction of the light
  //    from whatever cell it hits.  Run from top to bottom and you are set!"

  // For that reason, I'm going to have to do 2d workgroups, starting from the top, going to the bottom.
  //   It might not be super quick at runtime, with all the calls to glMemoryBarrier, but the concept is relatively simple.

  for (int y = DIM-1; y >= 0; y--) //iterating through y, from top to bottom
  {
    // update y index
    glUniform1i(glGetUniformLocation(fakeGI_compute, "y_index"), y);

    // send the job, for one xz plane
    glDispatchCompute(DIM/8, 1, DIM/8);

    // wait for all those shader invocations to finish
    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
  }
}

void OpenGL_container::mash()
{
  // 'mash' combines color data and lighting data, stores it back in the color data, so that you can save a block with lighting applied
    glUseProgram(mash_compute);
    //this one just directly manipulates the color data

    glUniform1iv(glGetUniformLocation(mash_compute, "current"), 1, &location_of_current);
    glUniform1iv(glGetUniformLocation(mash_compute, "lighting"), 1, &location_of_light_buffer);

    glDispatchCompute(DIM/8, DIM/8, DIM/8);

    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

    //lighting_clear(0.2);    //reset lighting buffer so appearance does not change
}



void OpenGL_container::game_of_life_update()
{
//╔═╗┌─┐┌┬┐┌─┐  ┌─┐┌─┐  ╦  ┬┌─┐┌─┐
//║ ╦├─┤│││├┤   │ │├┤   ║  │├┤ ├┤
//╚═╝┴ ┴┴ ┴└─┘  └─┘└    ╩═╝┴└  └─┘
    glUseProgram(game_of_life_update_compute);

    //send the preveious texture handles
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous"), 1, &location_of_previous);
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous_mask"), 1, &location_of_previous_mask);

    //send the current texture handles
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current"), 1, &location_of_current);
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current_mask"), 1, &location_of_current_mask);

    glDispatchCompute(DIM/8, DIM/8, DIM/8);

    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
}

void OpenGL_container::wireworld_update()
{
//╦ ╦┬┬─┐┌─┐╦ ╦┌─┐┬─┐┬  ┌┬┐
//║║║│├┬┘├┤ ║║║│ │├┬┘│   ││
//╚╩╝┴┴└─└─┘╚╩╝└─┘┴└─┴─┘─┴┘
    glUseProgram(wireworld_update_compute);

    //send the preveious texture handles
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous"), 1, &location_of_previous);
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "previous_mask"), 1, &location_of_previous_mask);

    //send the current texture handles
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current"), 1, &location_of_current);
    glUniform1iv(glGetUniformLocation(mask_by_color_compute, "current_mask"), 1, &location_of_current_mask);

    glDispatchCompute(DIM/8, DIM/8, DIM/8);

    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
}



std::string OpenGL_container::vat(float flip, std::string rule, int initmode, glm::vec4 color0, glm::vec4 color1, glm::vec4 color2, float lambda, float beta, float mag)
{
  int dimension;

  // this is the first way I thought to handle the dimension, note that the data will be
  // of a dimension corresponding to 2^dimension+1, so you will be throwing away some of
  // the data, since the texture is going to be one less, just the power of two

  if(DIM == 32)
    dimension = 5;
  else if(DIM == 64)
    dimension = 6;
  else if(DIM == 128)
    dimension = 7;
  else if(DIM == 256)
    dimension = 8;
  else if(DIM == 512)
    dimension = 9;
 
  // need to add rule to the constructor - check for equality with 'r' or 'i' to do random or isingRandom, else interpret as a shortrule
  voxel_automata_terrain v(dimension, flip, rule, initmode, lambda, beta, mag);

  // pull out the texture data
  std::vector<unsigned char> loaded_bytes; // used the same way as load(), below

  // triple for-loop to pull the data out
  for (int x = 0; x < DIM; x++)
  {
    for (int y = 0; y < DIM; y++)
    {
      for (int z = 0; z < DIM; z++)
      {
        // append data with the colors specified as input
        glm::vec4 color;
        switch (v.state[x][y][z])
        {
          case 0: color = color0; break; // use color0
          case 1: color = color1; break; // use color1
          case 2: color = color2; break; // use color2

          default: color = color0; break; // this shouldn't come up, but the compiler was mad
        }

        // put it in the vector as bytes
        loaded_bytes.push_back(static_cast<unsigned char>(color.x * 255));
        loaded_bytes.push_back(static_cast<unsigned char>(color.y * 255));
        loaded_bytes.push_back(static_cast<unsigned char>(color.z * 255));
        loaded_bytes.push_back(static_cast<unsigned char>(color.w * 255));
      }
    }
  }

  // send it
  glBindTexture(GL_TEXTURE_3D, block_textures[location_of_current]); // use the specified ID
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &loaded_bytes[0]);

  // get the rule out of v
  return v.getShortRule();
}

void OpenGL_container::load(std::string filename)
{
    std::vector<unsigned char> image_loaded_bytes;
    unsigned width, height;

    unsigned error = lodepng::decode(image_loaded_bytes, width, height, filename.c_str());

    //report any errors
    if(error) std::cout << "decode error during load(\" "+ filename +" \") " << error << ": " << lodepng_error_text(error) << std::endl;

    //put that shit in the front buffer with glTexImage3D()
    glBindTexture(GL_TEXTURE_3D, block_textures[location_of_current]); // use the specified ID
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, DIM, DIM, DIM, 0,  GL_RGBA, GL_UNSIGNED_BYTE, &image_loaded_bytes[0]);

    cout << "filename on load is: " << filename << std::endl << std::endl;
    
}

void OpenGL_container::save(std::string filename)
{
    std::vector<unsigned char> image_bytes_to_save;
    unsigned width, height;

    width = DIM;
    height = DIM*DIM;

    image_bytes_to_save.resize(4*DIM*DIM*DIM);
    filename = std::string("saves/") + filename; 

    //get that shit from the front buffer with glGetTexImage(), put it in image_bytes_to_save
    glGetTextureImage( block_textures[location_of_current], 0, GL_RGBA, GL_UNSIGNED_BYTE, 4*DIM*DIM*DIM, &image_bytes_to_save[0]); 
    
    unsigned error = lodepng::encode(filename.c_str(), image_bytes_to_save, width, height);

    if(error) std::cout << "decode error during save(\" "+ filename +" \") " << error << ": " << lodepng_error_text(error) << std::endl;

    cout << "filename on save is: " << filename << std::endl << std::endl;

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

  glUniform4fv(glGetUniformLocation(main_display_shader, "clear_color"),  1, glm::value_ptr(clear_color));

  glUniform1iv(glGetUniformLocation(main_display_shader, "current"),      1, &location_of_current);
  glUniform1iv(glGetUniformLocation(main_display_shader, "lighting"),     1, &location_of_light_buffer);

  glDrawArrays( GL_TRIANGLES, 0, 6 );
}
