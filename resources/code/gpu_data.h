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
    void swap_blocks();

    void display();

    void draw_aabb(glm::vec3 min, glm::vec3 max, glm::vec4 color, bool draw, bool mask);                                            //invokes the aabb compute shader
    void draw_sphere(glm::vec3 location, float radius, glm::vec4 color, bool draw, bool mask);                                      //invokes the sphere compute shader
    void draw_cylinder(glm::vec3 bvec, glm::vec3 tvec, float radius, glm::vec4 color, bool draw, bool mask);                        //invokes the cylinder compute shader
    void draw_tube(glm::vec3 bvec, glm::vec3 tvec, float inner_radius, float outer_radius, glm::vec4 color, bool draw, bool mask);  //invokes the tube compute shader


    //parameter lists tbd               shader exists | shader compiled | function written|
    void draw_cuboid();               //      x       |       -         |       -         | drawing cuboids
    void draw_ellipsoid();            //      x       |       -         |       -         | drawing ellipsoids
    void draw_heightmap();            //      x       |       -         |       -         | drawing heightmaps
    void draw_perlin_noise();         //      x       |       -         |       -         | drawing perlin noise
    void draw_triangle();             //      x       |       -         |       -         | drawing triangles

    // structure changes here - less shapes, more general operations
    void draw_blur();                 //      -       |       -         |       -         | variable radius blurring operation
    void clear_all();                 //      -       |       -         |       -         | clear all cells, respect mask value
    void unmask_all();                //      -       |       -         |       -         | unmask all cells
    void toggle_mask();               //      -       |       -         |       -         | toggle the mask value of all cells
    void mask_by_color();             //      -       |       -         |       -         | mask based on value/variance per channel
    void compute_static_lighting();   //      -       |       -         |       -         | compute static lighting - structure tbd
    void compute_ambient_occlusion(); //      -       |       -         |       -         | compute ambient occlusion - based on neighborhood
    void game_of_life_update();       //      -       |       -         |       -         | CA update, using GoL rules
    void wireworld_update();          //      -       |       -         |       -         | CA update, using WireWorld rules


    float scale;  //these need to be public so I can access them from above, in the voraldo class member functions
    float phi;
    float theta;

    glm::vec4 clear_color;

  private:
    GLuint vao; //vertex array object
    GLuint vbo; //vertex buffer object

    std::vector<glm::vec3> points;


  //╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐┌─┐
  //╚═╗├─┤├─┤ ││├┤ ├┬┘└─┐
  //╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─└─┘
    //display vertex/fragment shader pair
    GLuint main_display_shader;

    //compute shaders to manipulate the image objects   | WORKING |
    GLuint sphere_compute;                      //      |    x    |
    GLuint aabb_compute;                        //      |    x    |
    GLuint cylinder_compute;                    //      |    x    |
    GLuint cuboid_compute;                      //      |    -    |
    GLuint ellipsoid_compute;                   //      |    -    |
    GLuint heightmap_compute;                   //      |    -    |
    GLuint perlin_noise_compute;                //      |    -    |
    GLuint triangle_compute;                    //      |    -    |
    GLuint tube_compute;                        //      |    -    |

    GLuint blur_compute;                        //      |    -    |
    GLuint clear_all_compute;                   //      |    -    |
    GLuint unmask_all_compute;                  //      |    -    |
    GLuint toggle_mask_compute;                 //      |    -    |
    GLuint mask_by_color_compute;               //      |    -    |
    GLuint static_lighting_compute;             //      |    -    |
    GLuint ambient_occlusion_compute;           //      |    -    |
    GLuint game_of_life_update_compute;         //      |    -    |
    GLuint wireworld_update_compute;            //      |    -    |


  //╔╦╗┌─┐─┐ ┬┌┬┐┬ ┬┬─┐┌─┐┌─┐
  // ║ ├┤ ┌┴┬┘ │ │ │├┬┘├┤ └─┐
  // ╩ └─┘┴ └─ ┴ └─┘┴└─└─┘└─┘

    GLint location_of_current;
    GLint location_of_current_mask;

    GLint location_of_previous;
    GLint location_of_previous_mask;



    GLuint block_textures[2];
    GLuint mask_textures[2];
    GLuint perlin_texture;
    GLuint heightmap_texture;


};

#endif
