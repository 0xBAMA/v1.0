#include "voraldo1_0.h"

//this is where stuff like the init functions will all be defined

void voraldo::create_window()
{
    cout << "╦  ╦┌─┐┬─┐┌─┐┬  ┌┬┐┌─┐" << endl;
    cout << "╚╗╔╝│ │├┬┘├─┤│   │││ │" << endl;
    cout << " ╚╝ └─┘┴└─┴ ┴┴─┘─┴┘└─┘ v1.0" << endl;

  cout << endl << "Creating OpenGL window ...";

  if(SDL_Init( SDL_INIT_EVERYTHING ) != 0)
  {
      printf("Error: %s\n", SDL_GetError());
  }

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

  // GL 4.5 + GLSL 450
  const char* glsl_version = "#version 450";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );

  //this is how you query the screen resolution
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);

  //pulling these out because I'm going to try to span the whole screen with
  //the window, in a way that's flexible on different resolution screens
  int total_screen_width = dm.w;
  int total_screen_height = dm.h;

  window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS );
  GLcontext = SDL_GL_CreateContext( window );

  SDL_GL_MakeCurrent(window, GLcontext);
  SDL_GL_SetSwapInterval(1); // Enable vsync -- questionable utility
  // SDL_GL_SetSwapInterval(0); // explicitly disable vsync



  if (glewInit() != GLEW_OK)
  {
      fprintf(stderr, "Failed to initialize OpenGL loader!\n");
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io; //void cast prevents unused variable warning


  ImGui::StyleColorsDark();


  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, GLcontext);
  ImGui_ImplOpenGL3_Init(glsl_version);



  clear_color = ImVec4(0.26f, 0.16f, 0.0f, 0.5f);

  if(SDL_SetWindowOpacity(window, 1.0) == -1)   //0 is transparent 1 is opaque
    cout << "transparency not supported" << endl;


  //really excited by the fact imgui has an hsv picker to set this
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( window );

  cout << " done." << endl;

}

void voraldo::draw_menu_and_take_input()
{



  ImGuiIO& io = ImGui::GetIO();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();





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

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,230));
    ImGui::Begin("Voraldo 1.0", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Draw Menu", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = DRAW_MENU;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Mask Menu", ImVec2(120, 22)))
      current_menu_state = MASK_MENU;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Light Menu", ImVec2(120, 22)))
      current_menu_state = LIGHT_MENU;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("CA Menu", ImVec2(120, 22)))
      current_menu_state = CA_MENU;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Utility Menu", ImVec2(120, 22)))
      current_menu_state = UTIL_MENU;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("EXIT", ImVec2(120, 22)))
      current_menu_state = EXIT;


    ImGui::SetCursorPosX(37);
    ImGui::Text(" %.2f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::SetCursorPosX(40);
    ImGui::ColorEdit3("", (float*)&clear_color); // Edit 3 floats representing a color
    goto done;


  draw_menu_labels:

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,360));
    ImGui::Begin("Draw Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Sphere", ImVec2(120, 22)))
      current_menu_state = SPHERE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Perlin Noise", ImVec2(120, 22)))
      current_menu_state = PERLIN_NOISE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Triangle", ImVec2(120, 22)))
      current_menu_state = TRIANGLE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Ellipsoid", ImVec2(120, 22)))
      current_menu_state = ELLIPSOID_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Cylinder", ImVec2(120, 22)))
      current_menu_state = CYLINDER_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Tube", ImVec2(120, 22)))
      current_menu_state = TUBE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Cuboid", ImVec2(120, 22)))
      current_menu_state = CUBOID_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("AABB", ImVec2(120, 22)))
      current_menu_state = AABB_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Heightmap", ImVec2(120, 22)))
      current_menu_state = HEIGHTMAP_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Blur config", ImVec2(120, 22)))
      current_menu_state = BLUR_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Clear all config", ImVec2(120, 22)))
      current_menu_state = CLEAR_ALL_CONFIG;



    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(325);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;


    goto done;


  mask_menu_labels:


    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,180));
    ImGui::Begin("Mask Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Unmask All", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = UNMASK_ALL_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Toggle Mask", ImVec2(120, 22)))
      current_menu_state = TOGGLE_MASK_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Mask by Color", ImVec2(120, 22)))
      current_menu_state = MASK_BY_COLOR_CONFIG;


    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(145);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;


    goto done;


  light_menu_labels:

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,160));
    ImGui::Begin("Light Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Static", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = COMPUTE_STATIC_LIGHTING_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Per Frame", ImVec2(120, 22)))
      current_menu_state = PER_FRAME_LIGHTING_CONFIG;


    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(125);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;


  ca_menu_labels:

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,180));
    ImGui::Begin("Cellular Automata Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Game of Life", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = GAME_OF_LIFE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("WireWorld", ImVec2(120, 22)))
      current_menu_state = WIREWORLD_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("CA Terrain", ImVec2(120, 22)))
      current_menu_state = CA_TERRAIN_CONFIG;


    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(145);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;


  util_menu_labels:

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(220,180));
    ImGui::Begin("Utility Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Load/Save", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = LOAD_SAVE_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Reinitialize", ImVec2(120, 22)))
      current_menu_state = REINITIALIZATION_CONFIG;

    ImGui::SetCursorPosX(50);
    if (ImGui::Button("Undo", ImVec2(120, 22)))
      current_menu_state = UNDO_LAST_ACTION_CONFIG;


    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(145);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;


  sphere_config_labels:
    //radius, position, color, bool draw, bool mask
    goto done;


  perlin_noise_config_labels:
    //set scale, etc, and offer the option to load that new one into texture memory (or should we look at a compute shader that does it?)
    goto done;


  triangle_config_labels:
    // three point positions, thickness, color, bool draw, bool mask
    goto done;

  ellipsoid_config_labels:
    //radii, (rotation?), position, color, bool draw, bool mask
    goto done;


  cylinder_config_labels:
    //radius, bvec, tvec positions, color, bool draw, bool mask
    goto done;


  tube_config_labels:
    //inner radius, outer radius, bvec, tvec positions, color, bool draw, bool mask
    goto done;


  cuboid_config_labels:
    //8 points defining corners, color, bool draw, bool mask
    goto done;


  aabb_config_labels:
    //min/max on x/y/z, color, bool draw, bool mask
    goto done;


  heightmap_config_labels:
    //option to generate new (different algorithms), coloration, draw, mask
    goto done;


  blur_config_labels:
    //blur radius, bool touch alpha (zero alpha cells will stay invisible)
    goto done;


  clear_all_config_labels:
    //clear, like reinit but respects the mask values of all cells
    goto done;

  unmask_all_config_labels:
    //sets mask value for all cells to zero
    goto done;


  toggle_mask_config_labels:
    //toggles the value of mask for all cells
    goto done;


  mask_by_color_config_labels:
    //base value and range for r/g/b/a - powerful tool
    goto done;


  compute_static_lighting_config_labels:
    //single pass, static lighting
    goto done;


  per_frame_lighting_config_labels:
    //toggle a bool to make the lighting happen every frame
    goto done;


  game_of_life_config_labels:
    //single step or toggle bool to make it happen per frame
    goto done;


  wireworld_config_labels:
    //single step or toggle bool to make it happen per frame
    goto done;


  ca_terrain_config_labels:
    //need to look into how this is implemented -
    // https://softologyblog.wordpress.com/2017/05/27/voxel-automata-terrain/
    // https://bitbucket.org/BWerness/voxel-automata-terrain/src/master/
    goto done;


  load_save_config_labels:
    //lets you enter filenames and load/save from/to file
    goto done;


  reinitialization_config_labels:
    //like clear all, but ignores mask values
    goto done;


  undo_last_action_config_labels:
    //use multiple textures to support an undo operation
    goto done;











  done:
    //finished the labels for the current window, so do the end() thing and render it





  ImGui::End();

  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   //from hsv picker
  glClear(GL_COLOR_BUFFER_BIT);                     //clear the background

  //do my OpenGL raycasting here, over the cleared background

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());   //put imgui data into the framebuffer
  SDL_GL_SwapWindow(window);                      //swap the double buffers to display

  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
  // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        current_menu_state = EXIT;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        current_menu_state = EXIT;
    if (event.type == SDL_KEYUP  && event.key.keysym.sym == SDLK_ESCAPE)
        current_menu_state = EXIT;
  }

}

void voraldo::gl_data_setup()
{

}

void voraldo::quit()
{
  //blow away all the OpenGL data

  //destroy window

  //fullscreen exit splash? maybe if it's a quick one - I really liked that glitchy blinking eye gif

  //shutdown everything
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  cout << "GOODBYE" << endl << endl; //last line of code before main's return statement
}
