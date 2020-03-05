//  ╔╦╗┌─┐┌┐┌┬ ┬  ╦ ╦┌─┐┌┐┌┌┬┐┬  ┬┌┐┌┌─┐
//  ║║║├┤ ││││ │  ╠═╣├─┤│││ │││  │││││ ┬
//  ╩ ╩└─┘┘└┘└─┘  ╩ ╩┴ ┴┘└┘─┴┘┴─┘┴┘└┘└─┘


typedef enum menu_state_t
{
  EXIT,

  MAIN_MENU,    //top level menu - go to this from the splash

  DRAW_MENU,    //first level submenus
  MASK_MENU,
  LIGHT_MENU,
  CA_MENU,
  UTIL_MENU,

  //primitive config submenus
  SPHERE_CONFIG,
  PERLIN_NOISE_CONFIG,
  TRIANGLE_CONFIG,
  ELLIPSOID_CONFIG,
  CYLINDER_CONFIG,
  TUBE_CONFIG,
  CUBOID_CONFIG,
  AABB_CONFIG,
  HEIGHTMAP_CONFIG,
  BLUR_CONFIG,
  CLEAR_ALL_CONFIG,

  //mask config submenus
  UNMASK_ALL_CONFIG,
  TOGGLE_MASK_CONFIG,
  MASK_BY_COLOR_CONFIG,     //this is going to be a powerful tool

  //lighting config submenus
  COMPUTE_STATIC_LIGHTING_CONFIG,
  PER_FRAME_LIGHTING_CONFIG,

  //cellular automata submenus
  GAME_OF_LIFE_CONFIG,
  WIREWORLD_CONFIG,
  CA_TERRAIN_CONFIG,    //a la Visions of Chaos

  //utility submenus
  LOAD_SAVE_CONFIG,
  REINITIALIZATION_CONFIG,
  UNDO_LAST_ACTION_CONFIG

} menu_state;
