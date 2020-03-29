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

    void draw_sphere(glm::vec3 location, float radius, glm::vec4 color, bool draw, bool mask); //invokes the sphere compute shader

    //parameter lists tbd               shader exists | shader compiled | function written|
    void draw_perlin_noise();         //      x       |       -         |       -         |
    void draw_triangle();             //      x       |       -         |       -         |
    void draw_ellipsoid();            //      x       |       -         |       -         |
    void draw_cylinder();             //      x       |       -         |       -         |
    void draw_tube();                 //      x       |       -         |       -         |
    void draw_cuboid();               //      x       |       -         |       -         |
    void draw_aabb();                 //      x       |       -         |       -         |
    void draw_heightmap();            //      x       |       -         |       -         |
    void draw_blur();                 //      -       |       -         |       -         |
    void clear_all();                 //      -       |       -         |       -         |
    void unmask_all();                //      -       |       -         |       -         |
    void toggle_mask();               //      -       |       -         |       -         |
    void mask_by_color();             //      -       |       -         |       -         |
    void compute_static_lighting();   //      -       |       -         |       -         |
    void compute_ambient_occlusion(); //      -       |       -         |       -         |
    void game_of_life_update();       //      -       |       -         |       -         |
    void wireworld_update();          //      -       |       -         |       -         |


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

    //compute shaders to manipulate the image objects
    GLuint sphere_compute;
    GLuint perlin_noise_compute;
    GLuint triangle_compute;
    GLuint ellipsoid_compute;
    GLuint cylinder_compute;
    GLuint tube_compute;
    GLuint cuboid_compute;
    GLuint aabb_compute;
    GLuint heightmap_compute;
    GLuint blur_compute;
    GLuint clear_all_compute;
    GLuint unmask_all_compute;
    GLuint toggle_mask_compute;
    GLuint mask_by_color_compute;
    GLuint static_lighting_compute;
    GLuint ambient_occlusion_compute;
    GLuint game_of_life_update_compute;
    GLuint wireworld_update_compute;


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
