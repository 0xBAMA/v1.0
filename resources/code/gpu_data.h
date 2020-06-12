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

    void draw_aabb(glm::vec3 min, glm::vec3 max, glm::vec4 color, bool draw, bool mask);                                                                              //invokes the aabb compute shader
    void draw_sphere(glm::vec3 location, float radius, glm::vec4 color, bool draw, bool mask);                                                                        //invokes the sphere compute shader
    void draw_cylinder(glm::vec3 bvec, glm::vec3 tvec, float radius, glm::vec4 color, bool draw, bool mask);                                                          //invokes the cylinder compute shader
    void draw_tube(glm::vec3 bvec, glm::vec3 tvec, float inner_radius, float outer_radius, glm::vec4 color, bool draw, bool mask);                                    //invokes the tube compute shader
    void draw_cuboid(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 e, glm::vec3 f, glm::vec3 g, glm::vec3 h, glm::vec4 color, bool draw, bool mask);  //invokes the cuboid compute shader
    void draw_triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, float thickness, glm::vec4 color, bool draw, bool mask);                                 //invokes the triangle compute shader
    void draw_grid(glm::ivec3 spacing, glm::ivec3 widths, glm::vec4 color, bool draw, bool mask);                                                                     //invokes the triangle compute shader
    void draw_ellipsoid(glm::vec3 center, glm::vec3 radii, glm::vec3 rotation, glm::vec4 color, bool draw, bool mask);                                                //invokes the ellipsoid compute shader
    void draw_heightmap(float height_scale, bool height_color, glm::vec4 color, bool mask, bool draw);
    void draw_perlin_noise(float low_thresh, float high_thresh, glm::vec4 color, bool draw, bool mask);                                                               //drawing perlin noise - smooth or solid color? another bool

    void clear_all(bool respect_mask);                                                                                                                                //invokes the clear_all compute shader
    void unmask_all();                                                                                                                                                //invokes the unmask_all compute shader
    void toggle_mask();                                                                                                                                               //invokes the toggle_mask compute shader
    void blur(int radius, bool touch_alpha, bool respect_mask);                                                                                                       //invokes the blurring operation
    void mask_by_color(bool r, bool g, bool b, bool a, glm::vec4 color, float r_var, float g_var, float b_var, float a_var);             //mask based on value/variance per channel
    
    void compute_static_lighting(float theta, float phi, float ground_intensity, float initial_ray_intensity);   //compute static lighting - structure tbd
    void lighting_clear(float intensity);
    void compute_ambient_occlusion(); //compute ambient occlusion - based on neighborhood
    void game_of_life_update();       //CA update, using GoL rules
    void wireworld_update();          //CA update, using WireWorld rules

    //functions to generate new heightmaps
    void generate_heightmap_diamond_square();
    void generate_heightmap_perlin();
    void generate_heightmap_XOR();

    void generate_perlin_noise(float xscale, float yscale, float zscale);


    void load(std::string filename);
    void save(std::string filename);


    void shift(glm::ivec3 movement, bool loop, int mode);



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
    GLuint cuboid_compute;                      //      |    x    |
    GLuint ellipsoid_compute;                   //      |    x    |
    GLuint heightmap_compute;                   //      |    x    |
    GLuint perlin_noise_compute;                //      |    x    |
    GLuint triangle_compute;                    //      |    x    |
    GLuint grid_compute;                        //      |    x    |
    GLuint tube_compute;                        //      |    x    |

    GLuint blur_compute;                        //      |    x    |
    GLuint clear_all_compute;                   //      |    x    |
    GLuint shift_compute;
    GLuint unmask_all_compute;                  //      |    x    |
    GLuint toggle_mask_compute;                 //      |    x    |
    GLuint mask_by_color_compute;               //      |    x    |
    GLuint lighting_clear_compute;              //      |    -    |
    GLuint static_lighting_compute;             //      |    -    |
    GLuint ambient_occlusion_compute;           //      |    -    |
    GLuint game_of_life_update_compute;         //      |    -    |
    GLuint wireworld_update_compute;            //      |    -    |






  //╦┌┬┐┌─┐┌─┐┌─┐┌─┐
  //║│││├─┤│ ┬├┤ └─┐
  //╩┴ ┴┴ ┴└─┘└─┘└─┘

    GLint location_of_current;
    GLint location_of_current_mask;

    GLint location_of_previous;
    GLint location_of_previous_mask;

    GLint location_of_light_buffer;


  //╔╦╗┌─┐─┐ ┬┌┬┐┬ ┬┬─┐┌─┐┌─┐
  // ║ ├┤ ┌┴┬┘ │ │ │├┬┘├┤ └─┐
  // ╩ └─┘┴ └─ ┴ └─┘┴└─└─┘└─┘

    GLint location_of_perlin_noise;
    GLint location_of_heightmap;



    GLuint block_textures[2];
    GLuint mask_textures[2];
    GLuint light_texture;
    
    GLuint perlin_texture;
    GLuint heightmap_texture;

};

#endif
