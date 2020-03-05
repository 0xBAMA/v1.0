#include "voraldo1_0.h"

//this is where stuff like the init functions will all be defined

void voraldo::sdl_ttf_init()
{
  cout << "╦  ╦┌─┐┬─┐┌─┐┬  ┌┬┐┌─┐" << endl;
  cout << "╚╗╔╝│ │├┬┘├─┤│   │││ │" << endl;
  cout << " ╚╝ └─┘┴└─┴ ┴┴─┘─┴┘└─┘ v1.0" << endl;
  //initialize the text engine, load a local .ttf file, report error if neccesary
  if( TTF_Init() == -1 )
    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;

  if((font = TTF_OpenFont( "resources/Braciola MS.ttf", 14 )) == NULL)
    cout << "Font loading failed." << endl;
  else
    cout << "Font initialization done." << endl;
}

void voraldo::create_gl_window()
{

  cout << endl << "Creating OpenGL window ...";

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

  //this is how you query the screen resolution
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);

  //pulling these out because I'm going to try to span the whole screen with
  //the windows, without overlaps if possible, in a way that's flexible on different resolution screens
  int total_screen_width = dm.w;
  int total_screen_height = dm.h;

  OpenGL_window = SDL_CreateWindow( "OpenGL Window", total_screen_width/3, 0, 2*(total_screen_width/3), total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );
  GLcontext = SDL_GL_CreateContext( OpenGL_window );

  SDL_GL_MakeCurrent(OpenGL_window, GLcontext);

  glClearColor( 0.26, 0.16, 0.0, 1.0 );
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( OpenGL_window );

  cout << " done." << endl;

}

void voraldo::create_sdl_window()
{

  cout << endl << "Creating SDL window ...";

  //this is how you query the screen resolution
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);

  //pulling these out because I'm going to try to span the whole screen with
  //the windows, without overlaps if possible, in a way that's flexible on different resolution screens
  int total_screen_width = dm.w;
  int total_screen_height = dm.h;

  sdl_win_x = total_screen_width/3;
  sdl_win_y = total_screen_height;

  SDL_window = SDL_CreateWindow("Voraldo", 0, 0, total_screen_width/3, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );
  SDL_2D_renderer = SDL_CreateRenderer(SDL_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_SetRenderDrawColor(SDL_2D_renderer, 128, 125, 110, 255);
  SDL_RenderClear(SDL_2D_renderer); //clear our background

  SDL_Rect SrcRect  = {0,0,1200,400};   //where are we taking pixels from when we load the splash screen
  SDL_Rect DestRect = {0,0,total_screen_width/3,(total_screen_width/3)/3};  //where are we putting the pixels we took from SrcRect?

  cout << " done." << endl;

  std::stringstream m;
  std::vector<std::string> startup_info;

  cout << endl << "System Info:" << endl;

  //PATH
  cout << " Program launched from: " << SDL_GetBasePath() << endl;
  m << " Program launched from: " << SDL_GetBasePath() << endl;

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear

  //PLATFORM
  cout << " Running on " << std::string(SDL_GetPlatform()) << endl;
  m << " Running on " << std::string(SDL_GetPlatform()) << endl;

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear

  //SCREEN RESOLUTION
  cout << " Screen resolution is: " << total_screen_width << " by " << total_screen_height << endl;
  m << " Screen resolution is: " << total_screen_width << " by " << total_screen_height << endl;

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear


  //LOGICAL CPUs (THREADS)
  cout << " Logical CPU cores: " << SDL_GetCPUCount() << endl;
  m << " Logical CPU cores: " << SDL_GetCPUCount() << endl;

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear


  //SYSTEM RAM
  cout << " System RAM: " << SDL_GetSystemRAM() << " MB" << endl;
  m << " System RAM: " << SDL_GetSystemRAM() << " MB" << endl;

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear

  //BATTERY INFO
  int secs, pct;
  switch (SDL_GetPowerInfo(&secs, &pct))
  {
    case SDL_POWERSTATE_UNKNOWN:
      cout << " Battery in undetermined state" << endl;
      m << " Battery in undetermined state" << endl;
      break;

    case SDL_POWERSTATE_ON_BATTERY:
      cout << " Operating on battery - " << pct << " percent remaining, estimated " << secs << "seconds" << endl;
      m << " Operating on battery - " << pct << " percent remaining, estimated " << secs << "seconds" << endl;
      break;

    case SDL_POWERSTATE_NO_BATTERY:
      cout << " No Battery Present" << endl;
      m << " No Battery Present" << endl;
      break;

    case SDL_POWERSTATE_CHARGING:
      cout << " Battery is charging - " << pct << " percent" << endl;
      m << " Battery is charging - " << pct << " percent" << endl;
      break;

    case SDL_POWERSTATE_CHARGED:
      cout << " Battery is fully charged" << endl;
      m << " Battery is fully charged" << endl;
      break;
  }

  startup_info.push_back(m.str());  //get the path on the info vector
  std::stringstream().swap(m);     //swap m with a default constructed stringstream to clear

  cout << endl << endl;

  std::string splash_path = std::string("resources/splash.bmp");
  SDL_Texture* splash;           //this is the start splash screen
  SDL_Surface* splashBMP;

  splashBMP = SDL_LoadBMP(splash_path.c_str());
  splash = SDL_CreateTextureFromSurface(SDL_2D_renderer, splashBMP);
  SDL_FreeSurface(splashBMP);

  SDL_RenderCopy(SDL_2D_renderer, splash, &SrcRect, &DestRect);

  int baseoffset = (total_screen_width/3)/3;

  ttf_string(std::string("SYSTEM INFO:"), 10, baseoffset, 240, 190, 0);
  ttf_string(startup_info[0], 10, baseoffset+20, 200, 170, 0);
  ttf_string(startup_info[1], 10, baseoffset+40, 200, 170, 0);
  ttf_string(startup_info[2], 10, baseoffset+60, 200, 170, 0);
  ttf_string(startup_info[3], 10, baseoffset+80, 200, 170, 0);
  ttf_string(startup_info[4], 10, baseoffset+100, 200, 170, 0);
  ttf_string(startup_info[5], 10, baseoffset+120, 200, 170, 0);

  ttf_string(std::string("INSTRUCTIONS:"), 10, baseoffset+240, 240, 190, 0);
  ttf_string(std::string("  hit the number/letter of the option you wish to select"), 10, baseoffset+270, 240, 190, 0);
  ttf_string(std::string("  hit escape to go up a level"), 10, baseoffset+290, 240, 190, 0);
  ttf_string(std::string("  hit escape on the main menu to exit"), 10, baseoffset+310, 240, 190, 0);


  ttf_string(std::string("HIT ENTER TO CONTINUE or ESCAPE TO EXIT..."), 10, baseoffset+440, 240, 190, 0);

  SDL_RenderPresent(SDL_2D_renderer); //swap buffers

  SDL_Event event;

  while(1)
  {
    SDL_PollEvent(&event);

    if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN)
      break;

    if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
    {
      current_menu_state = EXIT;
      break;
    }
  }

}

void voraldo::draw_menu()
{

  SDL_SetRenderDrawColor(SDL_2D_renderer, 30, 30, 30, 255);
  SDL_RenderClear(SDL_2D_renderer); //clear our background

  //I've got some major improvements planned using labels and goto statements
  SDL_Rect s = {5,5,sdl_win_x - 30,sdl_win_y - 50};
  int basex, basey;

  switch (current_menu_state)
  { //set basex and basey
    case MAIN_MENU:
      basex = 9;
      basey = 5;
      goto main_init;

    case MASK_MENU:
    case DRAW_MENU:
    case LIGHT_MENU:
    case CA_MENU:
    case UTIL_MENU:
      basex = 14;
      basey = 22;
      goto first_level_init;

    case SPHERE_CONFIG:
    case PERLIN_NOISE_CONFIG:
    case TRIANGLE_CONFIG:
    case ELLIPSOID_CONFIG:
    case CYLINDER_CONFIG:
    case TUBE_CONFIG:
    case CUBOID_CONFIG:
    case AABB_CONFIG:
    case HEIGHTMAP_CONFIG:
    case BLUR_CONFIG:
    case CLEAR_ALL_CONFIG:

    case UNMASK_ALL_CONFIG:
    case TOGGLE_MASK_CONFIG:
    case MASK_BY_COLOR_CONFIG:

    case COMPUTE_STATIC_LIGHTING_CONFIG:
    case PER_FRAME_LIGHTING_CONFIG:

    case GAME_OF_LIFE_CONFIG:
    case WIREWORLD_CONFIG:
    case CA_TERRAIN_CONFIG:

    case LOAD_SAVE_CONFIG:
    case REINITIALIZATION_CONFIG:
    case UNDO_LAST_ACTION_CONFIG:
      basex = 19;
      basey = 39;
      goto second_level_init;
  }


//there are three different types of menu to draw - main menu, one level deep, and two levels deep
main_init:
  SDL_SetRenderDrawColor(SDL_2D_renderer, 128, 125, 110, 255);  //bright
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  goto done_w_init;


first_level_init:
  SDL_SetRenderDrawColor(SDL_2D_renderer, 100, 100, 100, 255);  //somewhat dimmed
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  s = {10,22,sdl_win_x - 30,sdl_win_y - 50};

  SDL_SetRenderDrawColor(SDL_2D_renderer, 128, 125, 110, 255);  //bright
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  goto done_w_init;


second_level_init:
  SDL_SetRenderDrawColor(SDL_2D_renderer, 75, 75, 75, 255); //somewhat more dimmed
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  s = {10,22,sdl_win_x - 30,sdl_win_y - 50};

  SDL_SetRenderDrawColor(SDL_2D_renderer, 100, 100, 100, 255);  //somewhat dimmed
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  s = {15,39,sdl_win_x - 30,sdl_win_y - 50};

  SDL_SetRenderDrawColor(SDL_2D_renderer, 128, 125, 110, 255);  //bright
  SDL_RenderFillRect(SDL_2D_renderer, &s);
  SDL_SetRenderDrawColor(SDL_2D_renderer, 50, 50, 50, 255);   //border
  SDL_RenderDrawRect(SDL_2D_renderer, &s);

  goto done_w_init;



done_w_init:  //we picked a basex, basey and drew the menu backgrounds

  //in all cases, top menu is labeled 'Main Menu'
  ttf_string("MAIN MENU", 9, 5, 0, 0, 0);

  switch (current_menu_state)
  { //note that the draw menu case includes all child menus, likewise for mask, etc
    case DRAW_MENU:

    case SPHERE_CONFIG:
    case PERLIN_NOISE_CONFIG:
    case TRIANGLE_CONFIG:
    case ELLIPSOID_CONFIG:
    case CYLINDER_CONFIG:
    case TUBE_CONFIG:
    case CUBOID_CONFIG:
    case AABB_CONFIG:
    case HEIGHTMAP_CONFIG:
    case BLUR_CONFIG:
    case CLEAR_ALL_CONFIG:
      ttf_string("DRAW MENU", 14, 22, 0, 0, 0);
      break;

    case MASK_MENU:

    case UNMASK_ALL_CONFIG:
    case TOGGLE_MASK_CONFIG:
    case MASK_BY_COLOR_CONFIG:
      ttf_string("MASK MENU", 14, 22, 0, 0, 0);
      break;


    case LIGHT_MENU:

    case COMPUTE_STATIC_LIGHTING_CONFIG:
    case PER_FRAME_LIGHTING_CONFIG:
      ttf_string("LIGHTING MENU", 14, 22, 0, 0, 0);
      break;

    case CA_MENU:

    case GAME_OF_LIFE_CONFIG:
    case WIREWORLD_CONFIG:
    case CA_TERRAIN_CONFIG:
      ttf_string("CELLULAR AUTOMATA MENU", 14, 22, 0, 0, 0);
      break;

    case UTIL_MENU:

    case LOAD_SAVE_CONFIG:
    case REINITIALIZATION_CONFIG:
    case UNDO_LAST_ACTION_CONFIG:
      ttf_string("UTILITIES MENU", 14, 22, 0, 0, 0);
      break;
  }

//this switch is simplified with the use of gotos, the labels below are where all the labeling happens,
  switch (current_menu_state)     //using the ttf_string function to write everything out
  {                               //note that simplified here just means reduced line count
    case MAIN_MENU:               //top menu
      goto main_menu_labels;

    case DRAW_MENU:               //one level deep
      goto draw_menu_labels;
    case MASK_MENU:
      goto mask_menu_labels;
    case LIGHT_MENU:
      goto light_menu_labels;
    case CA_MENU:
      goto ca_menu_labels;
    case UTIL_MENU:
      goto util_menu_labels;

    case SPHERE_CONFIG:           //two levels deep
      goto sphere_config_labels;
    case PERLIN_NOISE_CONFIG:
      goto perlin_noise_config_labels;
    case TRIANGLE_CONFIG:
      goto triangle_config_labels;
    case ELLIPSOID_CONFIG:
      goto ellipsoid_config_labels;
    case CYLINDER_CONFIG:
      goto cylinder_config_labels;
    case TUBE_CONFIG:
      goto tube_config_labels;
    case CUBOID_CONFIG:
      goto cuboid_config_labels;
    case AABB_CONFIG:
      goto aabb_config_labels;
    case HEIGHTMAP_CONFIG:
      goto heightmap_config_labels;
    case BLUR_CONFIG:
      goto blur_config_labels;
    case CLEAR_ALL_CONFIG:
      goto clear_all_config_labels;

      //mask config submenus
    case UNMASK_ALL_CONFIG:
      goto unmask_all_config_labels;
    case TOGGLE_MASK_CONFIG:
      goto toggle_mask_config_labels;
    case MASK_BY_COLOR_CONFIG:     //this is going to be a powerful tool
      goto mask_by_color_config_labels;

      //lighting config submenus
    case COMPUTE_STATIC_LIGHTING_CONFIG:
      goto compute_static_lighting_config_labels;
    case PER_FRAME_LIGHTING_CONFIG:
      goto per_frame_lighting_config_labels;

      //cellular automata submenus
    case GAME_OF_LIFE_CONFIG:
      goto game_of_life_config_labels;
    case WIREWORLD_CONFIG:
      goto wireworld_config_labels;
    case CA_TERRAIN_CONFIG:
      goto ca_terrain_config_labels;

      //utility submenus
    case LOAD_SAVE_CONFIG:
      goto load_save_config_labels;
    case REINITIALIZATION_CONFIG:
      goto reinitialization_config_labels;
    case UNDO_LAST_ACTION_CONFIG:
      goto undo_last_action_config_labels;

    default:                      //shouldn't ever see this
      goto done;
  }


main_menu_labels:

  goto done;


draw_menu_labels:

  goto done;


mask_menu_labels:

  goto done;


light_menu_labels:

  goto done;


ca_menu_labels:

  goto done;


util_menu_labels:

  goto done;


sphere_config_labels:

  goto done;


perlin_noise_config_labels:

  goto done;


triangle_config_labels:

  goto done;

ellipsoid_config_labels:

  goto done;


cylinder_config_labels:

  goto done;


tube_config_labels:

  goto done;


cuboid_config_labels:

  goto done;


aabb_config_labels:

  goto done;


heightmap_config_labels:

  goto done;


blur_config_labels:

  goto done;


clear_all_config_labels:

  goto done;

unmask_all_config_labels:

  goto done;


toggle_mask_config_labels:

  goto done;


mask_by_color_config_labels:

  goto done;


compute_static_lighting_config_labels:

  goto done;


per_frame_lighting_config_labels:

  goto done;


game_of_life_config_labels:

  goto done;


wireworld_config_labels:

  goto done;


ca_terrain_config_labels:

  goto done;


load_save_config_labels:

  goto done;


reinitialization_config_labels:

  goto done;


undo_last_action_config_labels:

  goto done;











done:
  SDL_RenderPresent(SDL_2D_renderer); //swap buffers so the user sees what you just drew
  SDL_Delay(5000);
  current_menu_state = EXIT;

}

void voraldo::ttf_string(std::string s, int basex, int basey, unsigned char r, unsigned char g, unsigned char b)
{
  SDL_Color col = {r, g, b};
  SDL_Surface * message_surface = TTF_RenderText_Solid( font, s.c_str(), col );
  SDL_Texture * message_text = SDL_CreateTextureFromSurface( SDL_2D_renderer, message_surface );

  int wid = message_surface->w;
  int hei = message_surface->h;

  SDL_Rect renderQuad = { basex, basey, wid, hei };
  SDL_RenderCopy( SDL_2D_renderer, message_text, NULL, &renderQuad );
  SDL_FreeSurface( message_surface );
  SDL_DestroyTexture( message_text );
}

void voraldo::quit()
{
  //blow away all the OpenGL data

  //destroy windows

  //fullscreen exit splash? maybe if it's a quick one - I really liked that glitchy blinking eye gif

  SDL_Quit();
  TTF_CloseFont(font);

  cout << "GOODBYE" << endl << endl; //last line of code before main's return statement
}
