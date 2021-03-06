#include "includes.h"
#include "voraldo1_0.h"

//this is where stuff like the init functions will all be defined



//used in load/save operation to check extension
bool hasEnding(std::string fullString, std::string ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

bool hasPNG(std::string filename)
{
    return hasEnding(filename, std::string(".png"));
}




//got this from http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
//the intention is to read all the files in the current directory and put them in the array

#define LISTBOX_SIZE 256
std::vector<std::string> directory_strings;

struct path_leaf_string
{
    std::string operator()(const std::filesystem::directory_entry& entry) const
    {
        return entry.path().string();
    }
};

void update_listbox_items()
{
    directory_strings.clear();

    std::filesystem::path p("saves");
    std::filesystem::directory_iterator start(p);
    std::filesystem::directory_iterator end;

    std::transform(start, end, std::back_inserter(directory_strings), path_leaf_string());

    //sort these alphabetically
    std::sort(directory_strings.begin(), directory_strings.end());
}







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

  //SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);  //gl_SamplePosition has to be used in my shader in order for this to
  //SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);   //have any effect on the way my shader works (i.e. add to gl_FragCoord)

  // GL 4.5 + GLSL 450
  const char* glsl_version = "#version 430";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

  //this is how you query the screen resolution
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);

  //pulling these out because I'm going to try to span the whole screen with
  //the window, in a way that's flexible on different resolution screens
  int total_screen_width = dm.w;
  int total_screen_height = dm.h;

  //window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS );
  window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE );
  GLcontext = SDL_GL_CreateContext( window );

  SDL_GL_MakeCurrent(window, GLcontext);
  SDL_GL_SetSwapInterval(1); // Enable vsync -- questionable utility
  //SDL_GL_SetSwapInterval(0); // explicitly disable vsync


  if (glewInit() != GLEW_OK)
  {
      fprintf(stderr, "Failed to initialize OpenGL loader!\n");
  }

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  #define FPS_HISTORY_SIZE 45
  fps_history.resize(FPS_HISTORY_SIZE);   //initialize the array of fps values

  update_listbox_items();   //initialize the list of what's in the saves directory

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io; //void cast prevents unused variable warning

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, GLcontext);
  ImGui_ImplOpenGL3_Init(glsl_version);


//if you want to change colors - open the demo window and dump this, replace all lines because that's the easiest way to do it
  ImVec4* colors = ImGui::GetStyle().Colors;
  {

  colors[ImGuiCol_Text]                   = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_TextDisabled]           = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
  colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.07f, 0.07f, 0.94f);
  colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_Border]                 = ImVec4(0.09f, 0.09f, 0.09f, 0.45f);
  colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
  colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
  colors[ImGuiCol_FrameBgActive]          = ImVec4(0.32f, 0.35f, 0.39f, 0.67f);
  colors[ImGuiCol_TitleBg]                = ImVec4(0.44f, 0.07f, 0.07f, 1.00f);
  colors[ImGuiCol_TitleBgActive]          = ImVec4(0.50f, 0.11f, 0.11f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.33f, 0.07f, 0.07f, 1.00f);
  colors[ImGuiCol_MenuBarBg]              = ImVec4(0.17f, 0.08f, 0.08f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.12f, 0.11f, 0.11f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark]              = ImVec4(0.49f, 0.05f, 0.05f, 1.00f);
  colors[ImGuiCol_SliderGrab]             = ImVec4(0.49f, 0.05f, 0.05f, 1.00f);
  colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_Button]                 = ImVec4(0.49f, 0.05f, 0.05f, 1.00f);
  colors[ImGuiCol_ButtonHovered]          = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ButtonActive]           = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_Header]                 = ImVec4(0.44f, 0.00f, 0.00f, 0.31f);
  colors[ImGuiCol_HeaderHovered]          = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_HeaderActive]           = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_Separator]              = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
  colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
  colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
  colors[ImGuiCol_ResizeGrip]             = ImVec4(0.44f, 0.00f, 0.00f, 0.31f);
  colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_Tab]                    = ImVec4(0.44f, 0.00f, 0.00f, 0.31f);
  colors[ImGuiCol_TabHovered]             = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_TabActive]              = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
  colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
  colors[ImGuiCol_PlotLines]              = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.71f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.35f, 0.00f, 0.00f, 0.35f);
  colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

  }

  // clear_color = ImVec4(0.26f, 0.16f, 0.0f, 0.5f); //initial value for clear color
  clear_color = ImVec4(75.0f/255.0f, 75.0f/255.0f, 75.0f/255.0f, 0.5f); //initial value for clear color

  // if(SDL_SetWindowOpacity(window, 1.0) == -1)   //0 is transparent 1 is opaque
  //   cout << "transparency not supported" << endl;

  //really excited by the fact imgui has an hsv picker to set this
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( window );

  cout << " done." << endl;

}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void voraldo::draw_menu_and_take_input()
{

  ImGuiIO& io = ImGui::GetIO();

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   //from hsv picker
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     //clear the background

  GPU_Data.display();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();


  //maintaining history of fps values

  //push back - put in the new value
  fps_history.push_back(io.Framerate);

  //pop front - take out the oldest value
  fps_history.pop_front();


//  static bool show_demo_window = true;
//  if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);


  //this switch is simplified with the use of gotos, the labels below are where all the labeling happens,
    switch (current_menu_state)     //using the ttf_string function to write everything out
                                  //note that simplified here just means reduced line count
    {
      case MAIN_MENU:               //top menu
        goto main_menu_label;

      case DRAW_MENU:               //one level deep
        goto draw_menu_label;
      case MASK_MENU:
        goto mask_menu_label;
      case LIGHT_MENU:
        goto light_menu_label;
      case CA_MENU:
        goto ca_menu_label;
      case UTIL_MENU:
        goto util_menu_label;

      case SPHERE_CONFIG:           //two levels deep
        goto sphere_config_label;
      case PERLIN_NOISE_CONFIG:
        goto perlin_noise_config_label;
      case TRIANGLE_CONFIG:
        goto triangle_config_label;
      case GRID_CONFIG:
        goto grid_config_label;
      case ELLIPSOID_CONFIG:
        goto ellipsoid_config_label;
      case CYLINDER_CONFIG:
        goto cylinder_config_label;
      case TUBE_CONFIG:
        goto tube_config_label;
      case CUBOID_CONFIG:
        goto cuboid_config_label;
      case AABB_CONFIG:
        goto aabb_config_label;
      case HEIGHTMAP_CONFIG:
        goto heightmap_config_label;
      case BLUR_CONFIG:
        goto blur_config_label;
      case CLEAR_ALL_CONFIG:
        goto clear_all_config_label;

        //mask config submenus
      case UNMASK_ALL_CONFIG:
        goto unmask_all_config_label;
      case TOGGLE_MASK_CONFIG:
        goto toggle_mask_config_label;
      case MASK_BY_COLOR_CONFIG:     //this is going to be a powerful tool
        goto mask_by_color_config_label;

        //cellular automata submenus
      case GAME_OF_LIFE_CONFIG:
        goto game_of_life_config_label;
      case WIREWORLD_CONFIG:
        goto wireworld_config_label;
      case CA_TERRAIN_CONFIG:
        goto ca_terrain_config_label;

        //utility submenus
      case LOAD_SAVE_CONFIG:
        goto load_save_config_label;
      case SHIFT_CONFIG:
        goto shift_config_label;
      case LIMITER_CONFIG:
        goto limiter_config_label;

      default:                      //shouldn't ever see this
        goto done;
        break;
    }


  main_menu_label:
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,315));
    ImGui::Begin("Voraldo 1.0", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Draw Menu", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = DRAW_MENU;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Mask Menu", ImVec2(120, 22)))
      current_menu_state = MASK_MENU;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Light Menu", ImVec2(120, 22)))
      current_menu_state = LIGHT_MENU;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("CA Menu", ImVec2(120, 22)))
      current_menu_state = CA_MENU;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Utility Menu", ImVec2(120, 22)))
      current_menu_state = UTIL_MENU;

    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("EXIT", ImVec2(120, 22)))
      current_menu_state = EXIT;

    ImGui::SetCursorPosX(45);
    ImGui::ColorEdit3("", (float*)&clear_color); // Edit 3 floats representing a color

    // cout << clear_color.x << " " << clear_color.y << " " << clear_color.z << endl;
    GPU_Data.clear_color = glm::vec4(clear_color.x, clear_color.y, clear_color.z, 1.0);


    // ImGui::SetCursorPosX(60);
    // ImGui::Text(" %.2f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    // fps graph
    static float values[FPS_HISTORY_SIZE] = {};
    float average = 0;

    for(int n = 0; n < FPS_HISTORY_SIZE; n++)
    {
      values[n] = fps_history[n];
      average += fps_history[n];
    }

    average /= FPS_HISTORY_SIZE;
    char overlay[32];
    sprintf(overlay, "avg %.2f fps (%.2f ms)", average, 1000.0f/average);
    ImGui::PlotLines("", values, IM_ARRAYSIZE(values), 0, overlay, 0.0f, 100.0f, ImVec2(240,60));



    goto done;
  }

  draw_menu_label:
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,485));
    ImGui::Begin("Draw Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

    ImGui::Text("These are the shapes you can use ");
    ImGui::Text("to draw. Use draw and mask check ");
    ImGui::Text("boxes to set the behavior of each ");
    ImGui::Text("drawing operation. ");
    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Sphere", ImVec2(120, 22)))
      current_menu_state = SPHERE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Perlin Noise", ImVec2(120, 22)))
      current_menu_state = PERLIN_NOISE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Grid", ImVec2(120, 22)))
      current_menu_state = GRID_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Triangle", ImVec2(120, 22)))
      current_menu_state = TRIANGLE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Ellipsoid", ImVec2(120, 22)))
      current_menu_state = ELLIPSOID_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Cylinder", ImVec2(120, 22)))
      current_menu_state = CYLINDER_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Tube", ImVec2(120, 22)))
      current_menu_state = TUBE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Cuboid", ImVec2(120, 22)))
      current_menu_state = CUBOID_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("AABB", ImVec2(120, 22)))
      current_menu_state = AABB_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Heightmap", ImVec2(120, 22)))
      current_menu_state = HEIGHTMAP_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Blur config", ImVec2(120, 22)))
      current_menu_state = BLUR_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Clear all config", ImVec2(120, 22)))
      current_menu_state = CLEAR_ALL_CONFIG;

    ImGui::Text(" ");
    ImGui::SetCursorPosX(70);

    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;


    goto done;
  }

  mask_menu_label:
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,265));
    ImGui::Begin("Mask Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Masking is an operation that will ");
    ImGui::Text("prevent subsequent draws from  ");
    ImGui::Text("touching the masked voxel. These ");
    ImGui::Text("are some tools to manipulate the ");
    ImGui::Text("state of this mask buffer. ");
    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Unmask All", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = UNMASK_ALL_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Toggle Mask", ImVec2(120, 22)))
      current_menu_state = TOGGLE_MASK_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Mask by Color", ImVec2(120, 22)))
      current_menu_state = MASK_BY_COLOR_CONFIG;

    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;


    goto done;
  }

  light_menu_label:
  {
    static float clear_level;

    static float directional_theta;
    static float directional_phi;
    static float directional_intensity;

    static int AO_radius;

    static float GI_scale_factor = 0.028;
    static float GI_alpha_thresh = 0.010;
    static float GI_sky_intensity = 0.16;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(300,850));
    ImGui::Begin("Light Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)


    ImGui::Text("This is the lighting system of Voraldo -");
    ImGui::Text("it is held as another buffer which scales");
    ImGui::Text("the color values held in the regular");
    ImGui::Text("block.");
    ImGui::Text(" ");
    ImGui::Text("Directional lighting is applied in a");
    ImGui::Text("manner sort of similar to the shadow map");
    ImGui::Text("approach that is used elsewhere.");
    ImGui::Text(" ");



    ImGui::Text("Clear Level - 0.25 is neutral");
    ImGui::SliderFloat("level", &clear_level, 0.0f, 1.0f, "%.3f");

    if (ImGui::Button("Clear", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      GPU_Data.lighting_clear(clear_level);

    ImGui::Separator();

    ImGui::Text("Directional");
    ImGui::SliderFloat("theta", &directional_theta, -3.14f, 3.14f, "%.3f");
    ImGui::SliderFloat("phi", &directional_phi, -3.14f, 3.14f, "%.3f");
    ImGui::Text(" ");
    ImGui::SliderFloat("value", &directional_intensity, 0.0f, 1.0f, "%.3f");

    if (ImGui::Button("Apply Directional", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      GPU_Data.compute_static_lighting(directional_theta, directional_phi, directional_intensity);

    //if (ImGui::Button("Per Frame", ImVec2(120, 22)))
      //current_menu_state = PER_FRAME_LIGHTING_CONFIG;

    ImGui::Separator();

    ImGui::Text("Ambient occlusion is based on a weighted");
    ImGui::Text("average of the alpha values in the");
    ImGui::Text("specified size neighborhood.");
    ImGui::Text(" ");
    ImGui::SliderInt("radius", &AO_radius, 0, 5);

    if (ImGui::Button("Apply AO", ImVec2(120, 22)))
    {
        GPU_Data.compute_ambient_occlusion(AO_radius);
    }


    ImGui::Separator();

    ImGui::Text("Fake GI is computed by tracing rays");
    ImGui::Text("upwards from each cell. If they ");
    ImGui::Text("escape the volume, they get the");
    ImGui::Text("sky_intensity added. Otherwise they");
    ImGui::Text("take a portion of the light of the");
    ImGui::Text("cell they hit, set by sfactor.");

    ImGui::SliderFloat("sfactor", &GI_scale_factor, 0.0f, 1.0f);
    ImGui::SliderFloat("alpha threshold", &GI_alpha_thresh, 0.0f, 1.0f);
    ImGui::SliderFloat("sky intensity", &GI_sky_intensity, 0.0f, 1.0f);

    if(ImGui::Button("Apply GI", ImVec2(120, 22)))
    {
      GPU_Data.compute_fake_GI(GI_scale_factor, GI_sky_intensity, GI_alpha_thresh);
    }

    ImGui::Separator();
    ImGui::Text("Mash combines the lighting buffer");
    ImGui::Text("and the color buffer, so that the");
    ImGui::Text("block can be saved with the ");
    ImGui::Text("lighting applied.");

    if (ImGui::Button("Mash", ImVec2(120, 22)))
    {
        GPU_Data.mash();
    }

    ImGui::Separator();




    ImGui::Text(" ");
    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;
  }

  ca_menu_label:
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,195));
    ImGui::Begin("Cellular Automata Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Game of Life", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = GAME_OF_LIFE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("WireWorld", ImVec2(120, 22)))
      current_menu_state = WIREWORLD_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("CA Terrain", ImVec2(120, 22)))
      current_menu_state = CA_TERRAIN_CONFIG;


    ImGui::SetCursorPosX(70);
    ImGui::SetCursorPosY(145);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;

  }

  util_menu_label:
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,195));
    ImGui::Begin("Utility Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text(" ");

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Load/Save", ImVec2(120, 22))) // Buttons return true when clicked (most widgets return true when edited/activated)
      current_menu_state = LOAD_SAVE_CONFIG;

    ImGui::SetCursorPosX(70);
    if (ImGui::Button("Shifting", ImVec2(120, 22)))
      current_menu_state = SHIFT_CONFIG;


    ImGui::SetCursorPosX(70);
    ImGui::SetCursorPosY(145);
    if (ImGui::Button("Main Menu", ImVec2(120, 22)))
      current_menu_state = MAIN_MENU;

    goto done;

  }

  sphere_config_label:
    //required: radius, location, color, bool draw, bool mask
  {
    static bool sphere_draw = true, sphere_mask = false;
    static float sphere_radius = 0.0;
    static ImVec4 sphere_draw_color;
    static glm::vec3 sphere_location;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,295));
    ImGui::Begin("Sphere Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Use the sliders to set the radius");
    ImGui::Text("and the x, y, z components of the");
    ImGui::Text("center's position.");
    ImGui::Text(" ");

    ImGui::SliderFloat("  radius", &sphere_radius, 0.0f, 500.0f, "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("  x pos", &sphere_location.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  y pos", &sphere_location.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  z pos", &sphere_location.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &sphere_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &sphere_mask);

    ImGui::ColorEdit4("  Color", (float*)&sphere_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    // ImGui::SetCursorPosY(200);
    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the sphere with the selected values
        GPU_Data.draw_sphere(sphere_location, sphere_radius, glm::vec4(sphere_draw_color.x, sphere_draw_color.y, sphere_draw_color.z, sphere_draw_color.w), sphere_draw, sphere_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  perlin_noise_config_label:
    //set scale, etc, and offer the option to load that new one into texture memory (or should we look at a compute shader that does it?)
    // need something to generate a new perlin texture, put it on the gpu?
  {
    static float perlin_scale_x = 0.014;
    static float perlin_scale_y = 0.014;
    static float perlin_scale_z = 0.014;
    static float perlin_threshold_lo = 0.0f;
    static float perlin_threshold_hi = 0.0f;
    static ImVec4 perlin_draw_color;
    static bool perlin_draw = true;
    static bool perlin_mask = false;
    static bool perlin_smooth = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,430));
    ImGui::Begin("Perlin Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Larger numbers are smaller lobes.");
    ImGui::Text("Click generate to send your new ");
    ImGui::Text("scalings to the GPU to draw with.");
    ImGui::Text(" ");
    ImGui::SliderFloat("  xscale", &perlin_scale_x, 0.01f, 0.5f, "%.3f");
    ImGui::SliderFloat("  yscale", &perlin_scale_y, 0.01f, 0.5f, "%.3f");
    ImGui::SliderFloat("  zscale", &perlin_scale_z, 0.01f, 0.5f, "%.3f");
    ImGui::Text(" ");

    if(ImGui::Button("generate"))
    {
        GPU_Data.generate_perlin_noise(perlin_scale_x, perlin_scale_y, perlin_scale_z);
    }

    ImGui::Separator();

    ImGui::Text("Perlin noise ranges from 0 to 1.");
    ImGui::Text("Use hithresh and lowthresh to tell");
    ImGui::Text("how much of this perlin texture to");
    ImGui::Text("color in.");


    ImGui::SliderFloat(" hithresh", &perlin_threshold_hi, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat(" lothresh", &perlin_threshold_lo, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox(" Smooth Color ", &perlin_smooth);

    ImGui::Separator();


    ImGui::Checkbox("  Draw ", &perlin_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &perlin_mask);

    ImGui::ColorEdit4("  Color", (float*)&perlin_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");

    ImGui::SetCursorPosX(16);
    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw with the selected values
        GPU_Data.draw_perlin_noise(perlin_threshold_lo, perlin_threshold_hi, perlin_smooth, glm::vec4(perlin_draw_color.x, perlin_draw_color.y, perlin_draw_color.z, perlin_draw_color.w), perlin_draw, perlin_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  grid_config_label:
  {
    static int xoff, yoff, zoff;
    static int xspacing, yspacing, zspacing;
    static int xwid, ywid, zwid;
    static ImVec4 grid_draw_color;
    static bool grid_draw = true;
    static bool grid_mask = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,490));

    ImGui::Begin("Grid Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Use the spacing control to set the");
    ImGui::Text("distance between grid lines. Width");
    ImGui::Text("sets the width of these grid lines,");
    ImGui::Text("and offset allows the whole grid");
    ImGui::Text("to be moved. ");
    ImGui::Text(" ");
    ImGui::Text("spacing");
    ImGui::SliderInt(" xs", &xspacing, 0, 15);
    ImGui::SliderInt(" ys", &yspacing, 0, 15);
    ImGui::SliderInt(" zs", &zspacing, 0, 15);

    ImGui::Text("width");
    ImGui::SliderInt(" xw", &xwid, 0, 15);
    ImGui::SliderInt(" yw", &ywid, 0, 15);
    ImGui::SliderInt(" zw", &zwid, 0, 15);

    ImGui::Text("offset");
    ImGui::SliderInt(" xo", &xoff, 0, 15);
    ImGui::SliderInt(" yo", &yoff, 0, 15);
    ImGui::SliderInt(" zo", &zoff, 0, 15);

    ImGui::Checkbox("  Draw ", &grid_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &grid_mask);

    ImGui::ColorEdit4("  Color", (float*)&grid_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");


    ImGui::SetCursorPosX(16);
    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw with the selected values

        GPU_Data.draw_grid(glm::ivec3(xspacing, yspacing, zspacing), glm::ivec3(xwid, ywid, zwid), glm::ivec3(xoff, yoff, zoff), glm::vec4(grid_draw_color.x, grid_draw_color.y, grid_draw_color.z, grid_draw_color.w), grid_draw, grid_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }



  triangle_config_label:
    // three point positions, thickness, color, bool draw, bool mask
  {
    static float thickness;
    static glm::vec3 point1, point2, point3;
    static ImVec4 triangle_draw_color;
    static bool triangle_draw = true;
    static bool triangle_mask = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,460));
    ImGui::Begin("Triangle Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)


    ImGui::Text("Triangles consist of three points,");
    ImGui::Text("use the sliders below to set each");
    ImGui::Text("x, y and z value. Thickness will");
    ImGui::Text("set the thickness of the triangle.");
    ImGui::Text(" ");

    ImGui::SliderFloat(" thickness", &thickness, 0.0f, 300.0f, "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("  x1 ", &point1.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  y1 ", &point1.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  z1 ", &point1.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("  x2 ", &point2.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  y2 ", &point2.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  z2 ", &point2.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("  x3 ", &point3.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  y3 ", &point3.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("  z3 ", &point3.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &triangle_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &triangle_mask);

    ImGui::ColorEdit4("  Color", (float*)&triangle_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);


    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the triangle with the selected values
        GPU_Data.draw_triangle(point1, point2, point3, thickness, glm::vec4(triangle_draw_color.x, triangle_draw_color.y, triangle_draw_color.z, triangle_draw_color.w), triangle_draw, triangle_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  ellipsoid_config_label:
    //radii, (rotation?), position, color, bool draw, bool mask
  {
    static glm::vec3 radius, center, rotation;
    static bool ellipsoid_draw = true, ellipsoid_mask = false;
    static ImVec4 ellipsoid_draw_color;


    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,435));
    ImGui::Begin("Ellipsoid Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Ellipsoid is similar to the sphere");
    ImGui::Text("but has three different radii. In");
    ImGui::Text("addition to this it can be rotated");
    ImGui::Text("to give the desired orientation. ");
    ImGui::Text(" ");


    ImGui::SliderFloat("x location", &center.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("y location", &center.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("z location", &center.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("x radius", &radius.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("y radius", &radius.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("z radius", &radius.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("x rotation", &rotation.x, 0.0f, 6.28f, "%.3f");
    ImGui::SliderFloat("y rotation", &rotation.y, 0.0f, 6.28f, "%.3f");
    ImGui::SliderFloat("z rotation", &rotation.z, 0.0f, 6.28f, "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &ellipsoid_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &ellipsoid_mask);

    ImGui::ColorEdit4("  Color", (float*)&ellipsoid_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);



    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the ellipsoid with the selected values
        GPU_Data.draw_ellipsoid(center, radius, rotation, glm::vec4(ellipsoid_draw_color.x, ellipsoid_draw_color.y, ellipsoid_draw_color.z, ellipsoid_draw_color.w), ellipsoid_draw, ellipsoid_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  cylinder_config_label:
    //radius, bvec, tvec positions, color, bool draw, bool mask
  {
    static glm::vec3 cylinder_bvec, cylinder_tvec;
    static bool cylinder_draw = true, cylinder_mask = false;
    static ImVec4 cylinder_draw_color;
    static float cylinder_radius;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,400));
    ImGui::Begin("Cylinder Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Cylinder is defined by two points.");
    ImGui::Text("tvec is in the center of the top");
    ImGui::Text("and bvec is in the center of the");
    ImGui::Text("bottom. Thickness determines the");
    ImGui::Text("radius of the cylinder. ");
    ImGui::Text(" ");

    ImGui::SliderFloat(" radius", &cylinder_radius, 0.0f, 300.0f, "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("bvec x", &cylinder_bvec.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("bvec y", &cylinder_bvec.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("bvec z", &cylinder_bvec.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("tvec x", &cylinder_tvec.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("tvec y", &cylinder_tvec.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("tvec z", &cylinder_tvec.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &cylinder_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &cylinder_mask);

    ImGui::ColorEdit4("  Color", (float*)&cylinder_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the cylinder with the selected values
        GPU_Data.draw_cylinder(cylinder_bvec, cylinder_tvec, cylinder_radius, glm::vec4(cylinder_draw_color.x, cylinder_draw_color.y, cylinder_draw_color.z, cylinder_draw_color.w), cylinder_draw, cylinder_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  tube_config_label:
    //inner radius, outer radius, bvec, tvec positions, color, bool draw, bool mask
  {
    static glm::vec3 tube_bvec, tube_tvec;
    static bool tube_draw = true, tube_mask = false;
    static ImVec4 tube_draw_color;
    static float tube_inner_radius, tube_outer_radius;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,420));
    ImGui::Begin("Tube Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Tube is a cylinder with a cylinder");
    ImGui::Text("cut out from the center. Outer is");
    ImGui::Text("the outer radius, and inner is the");
    ImGui::Text("radius of the cutout.");
    ImGui::Text(" ");

    ImGui::Text("Radii");

    ImGui::SliderFloat("inner", &tube_inner_radius, 0.0f, 300.0f, "%.3f");
    ImGui::SliderFloat("outer", &tube_outer_radius, 0.0f, 300.0f, "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("bvec x", &tube_bvec.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("bvec y", &tube_bvec.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("bvec z", &tube_bvec.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("tvec x", &tube_tvec.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("tvec y", &tube_tvec.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("tvec z", &tube_tvec.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &tube_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &tube_mask);

    ImGui::ColorEdit4("  Color", (float*)&tube_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the cylinder with the selected values
        GPU_Data.draw_tube(tube_bvec, tube_tvec, tube_inner_radius, tube_outer_radius, glm::vec4(tube_draw_color.x, tube_draw_color.y, tube_draw_color.z, tube_draw_color.w), tube_draw, tube_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  cuboid_config_label:
    //8 points defining corners, color, bool draw, bool mask
  {
    //kind of start with some defaults because this is not super easy to use
    static glm::vec3 a = glm::vec3( 20,230,230);   // 	a : -x, +y, +z
    static glm::vec3 b = glm::vec3( 20, 20,230);   // 	b : -x, -y, +z
    static glm::vec3 c = glm::vec3(230,230,230);   // 	c : +x, +y, +z
    static glm::vec3 d = glm::vec3(230, 20,230);   // 	d : +x, -y, +z
    static glm::vec3 e = glm::vec3( 20,230, 20);   // 	e : -x, +y, -z
    static glm::vec3 f = glm::vec3( 20, 20, 20);   // 	f : -x, -y, -z
    static glm::vec3 g = glm::vec3(230,230, 20);   // 	g : +x, +y, -z
    static glm::vec3 h = glm::vec3(230, 20, 20);   // 	h : +x, -y, -z

    static bool cuboid_draw = true, cuboid_mask = false;
    static ImVec4 cuboid_draw_color;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,875));
    ImGui::Begin("Cuboid Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This is hard to use.");
    ImGui::Text("     e-------g    +y     ");
    ImGui::Text("    /|      /|     |     ");
    ImGui::Text("   / |     / |     |___+x");
    ImGui::Text("  a-------c  |    /      ");
    ImGui::Text("  |  f----|--h   +z      ");
    ImGui::Text("  | /     | /            ");
    ImGui::Text("  |/      |/             ");
    ImGui::Text("  b-------d              ");

    ImGui::Separator();

    ImGui::SliderFloat("a x", &a.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("a y", &a.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("a z", &a.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("b x", &b.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("b y", &b.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("b z", &b.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("c x", &c.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("c y", &c.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("c z", &c.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("d x", &d.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("d y", &d.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("d z", &d.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("e x", &e.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("e y", &e.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("e z", &e.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("f x", &f.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("f y", &f.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("f z", &f.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("g x", &g.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("g y", &g.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("g z", &g.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat("h x", &h.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("h y", &h.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat("h z", &h.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &cuboid_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &cuboid_mask);

    ImGui::ColorEdit4("  Color", (float*)&cuboid_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the cylinder with the selected values
        GPU_Data.draw_cuboid(a, b, c, d, e, f, g, h, glm::vec4(cuboid_draw_color.x, cuboid_draw_color.y, cuboid_draw_color.z, cuboid_draw_color.w), cuboid_draw, cuboid_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  aabb_config_label:
    //min/max on x/y/z, color, bool draw, bool mask
  {
    static glm::vec3 max, min;
    static ImVec4 aabb_draw_color;
    static bool aabb_draw = true, aabb_mask = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,365));
    ImGui::Begin("AABB Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)


    ImGui::Text("This is an axis-aligned bounding");
    ImGui::Text("box. You set a min and max on");
    ImGui::Text("each axis, and it will fill in");
    ImGui::Text("the rectangular prism inside.");
    ImGui::Text(" ");

    ImGui::SliderFloat(" x max", &max.x, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat(" x min", &min.x, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat(" y max", &max.y, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat(" y min", &min.y, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::SliderFloat(" z max", &max.z, 0.0f, float(DIM), "%.3f");
    ImGui::SliderFloat(" z min", &min.z, 0.0f, float(DIM), "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &aabb_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &aabb_mask);

    ImGui::ColorEdit4("  Color", (float*)&aabb_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the sphere with the selected values
        GPU_Data.draw_aabb(min, max, glm::vec4(aabb_draw_color.x, aabb_draw_color.y, aabb_draw_color.z, aabb_draw_color.w), aabb_draw, aabb_mask);
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  heightmap_config_label:
    //option to generate new (different algorithms), coloration, draw, mask
  {
    static float heightmap_vertical_scale = 1.0;
    static bool heightmap_draw = true, heightmap_mask = false;
    static ImVec4 heightmap_draw_color;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,575));
    ImGui::Begin("Heightmap Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This is a tool to draw heightmaps.");
    ImGui::Text("Use the options to generate new");
    ImGui::Text("ones, and use the vertical scale");
    ImGui::Text("to set the height scaling. ");
    ImGui::Text(" ");
    ImGui::Text("There are three types of maps you");
    ImGui::Text("can generate. ");
    ImGui::Text(" ");

    //show off the currently held texture
    ImGui::Image((void*)(intptr_t) 7   /*not a good way to do this, but it works right now*/ , ImVec2(240,256));




    if (ImGui::Button("perlin"))
    {
        GPU_Data.generate_heightmap_perlin();
    }

    ImGui::SameLine();

    if (ImGui::Button("diamond-square"))
    {
        GPU_Data.generate_heightmap_diamond_square();
    }

    ImGui::SameLine();

    if (ImGui::Button("XOR"))
    {
        GPU_Data.generate_heightmap_XOR();
    }


    ImGui::Separator();
    ImGui::SliderFloat(" Scale", &heightmap_vertical_scale, 0.0f, 5.0f, "%.3f");
    ImGui::Separator();

    ImGui::Checkbox("  Draw ", &heightmap_draw);
    ImGui::SameLine();
    ImGui::Checkbox("  Mask ", &heightmap_mask);

    ImGui::ColorEdit4("  Color", (float*)&heightmap_draw_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Draw", ImVec2(100, 22)))
    {
        //draw the heightmap with the selected values
        GPU_Data.draw_heightmap(heightmap_vertical_scale, true, glm::vec4(heightmap_draw_color.x, heightmap_draw_color.y, heightmap_draw_color.z, heightmap_draw_color.w), heightmap_mask, heightmap_draw);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  blur_config_label:
    //blur radius, bool touch alpha (zero alpha cells will stay invisible)
  {
    static int blur_radius = 0;
    static bool touch_alpha = true;
    static bool respect_mask = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,260));
    ImGui::Begin("Blur Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This is a simple box blur. It will ");
    ImGui::Text("consider the size neighborhood you ");
    ImGui::Text("select, and average the colors to ");
    ImGui::Text("give smoother transitions beteen ");
    ImGui::Text("neighboring cells. ");
    ImGui::Text(" ");

    ImGui::SliderInt(" Radius", &blur_radius, 0, 5);

    ImGui::Separator();

    ImGui::Checkbox("  Touch alpha ", &touch_alpha);
    ImGui::Checkbox("  Respect mask ", &respect_mask);



    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Blur", ImVec2(100, 22)))
    {
        //do the blur operation with the selected values
        GPU_Data.blur(blur_radius, touch_alpha, respect_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;
  }

  clear_all_config_label:
    //clear, like reinit but respects the mask values of all cells
  {
    static bool respect_mask = false;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,185));
    ImGui::Begin("Clear All Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This will clear the block, with an");
    ImGui::Text("option to respect the mask. If you ");
    ImGui::Text("check that, masked cells won't be ");
    ImGui::Text("cleared. ");
    ImGui::Text(" ");

    ImGui::Checkbox("  Respect mask ", &respect_mask);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Clear", ImVec2(100, 22)))
    {
        //do the clear all operation - note that this respects the mask values
        GPU_Data.clear_all(respect_mask);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = DRAW_MENU;

    goto done;

  }


  unmask_all_config_label:
    //sets mask value for all cells to zero
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,135));
    ImGui::Begin("Unmask All Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This will clear the mask value for");
    ImGui::Text("all cells. Equivalently, set mask ");
    ImGui::Text("to false for all voxels. ");
    ImGui::Text(" ");

    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Unmask All", ImVec2(100, 22)))
    {
        //unmask all cells
        GPU_Data.unmask_all();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = MASK_MENU;

    goto done;
  }

  toggle_mask_config_label:
    //toggles the value of mask for all cells
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,150));
    ImGui::Begin("Toggle Mask Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This will toggle the value of mask ");
    ImGui::Text("for all voxels. Masked cells will ");
    ImGui::Text("become unmasked, and unmasked will ");
    ImGui::Text("become masked. ");
    ImGui::Text(" ");

    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Toggle", ImVec2(100, 22)))
    {
        //do the toggle operation
        GPU_Data.toggle_mask();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = MASK_MENU;

    goto done;
  }

  mask_by_color_config_label:
    //base value and range for r/g/b/a - powerful tool
  {
    static bool use_r;
    static bool use_g;
    static bool use_b;
    static bool use_a;
    static bool use_l;

    static ImVec4 select_color;
    static float light_val=0.0001;

    static float r_variance=0.0;
    static float g_variance=0.0;
    static float b_variance=0.0;
    static float a_variance=0.0;
    static float l_variance=0.0;


    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,600));
    ImGui::Begin("Mask by Color Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("Use the HSV picker or the RGB fields");
    ImGui::Text("to enter a color. Once you do that, ");
    ImGui::Text("use the check boxes and sliders to ");
    ImGui::Text("express how you want to use each");
    ImGui::Text("channel. ");
    ImGui::Text(" ");
    ImGui::Text("For example, if I pick 255 in the");
    ImGui::Text("red channel, check the red check ");
    ImGui::Text("box, and set the slider to a non ");
    ImGui::Text("zero value, you will be masking ");
    ImGui::Text("the parts of the image that have ");
    ImGui::Text("a high value in the red channel. ");
    ImGui::Text(" ");
    ImGui::Text("The slider sets how broadly this ");
    ImGui::Text("operation will be applied. ");
    ImGui::Text(" ");
    ImGui::Text("This can be applied to the RGBA ");
    ImGui::Text("color channels as well as the ");
    ImGui::Text("value in the lighting buffer, to ");
    ImGui::Text("mask only light or dark areas. ");
    ImGui::Text(" ");


    ImGui::ColorEdit4("  Color", (float*)&select_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    //sliders

    ImGui::Checkbox("use r", &use_r);
    ImGui::SameLine();
    ImGui::SliderFloat("r variance", &r_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use g", &use_g);
    ImGui::SameLine();
    ImGui::SliderFloat("g variance", &g_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use b", &use_b);
    ImGui::SameLine();
    ImGui::SliderFloat("b variance", &b_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use a", &use_a);
    ImGui::SameLine();
    ImGui::SliderFloat("a variance", &a_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Separator();

    ImGui::Checkbox("use l", &use_l);
    ImGui::SameLine();
    ImGui::SliderFloat("l value", &light_val, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("l variance", &l_variance, 0.0f, 1.0f, "%.3f");

    if (ImGui::Button("Mask", ImVec2(100, 22)))
    {
        GPU_Data.mask_by_color(use_r, use_g, use_b, use_a, use_l, glm::vec4(select_color.x, select_color.y, select_color.z, select_color.w), light_val, r_variance, g_variance, b_variance, a_variance, l_variance);
    }

    ImGui::SameLine();

    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = MASK_MENU;


    goto done;
  }


  game_of_life_config_label:
    //single step or toggle bool to make it happen per frame
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,85));
    ImGui::Begin("Game of Life Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Advance", ImVec2(100, 22)))
    {
        //compute one update
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = CA_MENU;

    goto done;
  }

  wireworld_config_label:
    //single step or toggle bool to make it happen per frame
  {
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,85));
    ImGui::Begin("WireWorld Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Advance", ImVec2(100, 22)))
    {
        //compute one update
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = CA_MENU;

    //toggle per frame updates, too?

    goto done;
  }

  ca_terrain_config_label:
    //need to look into how this is implemented -
    // https://softologyblog.wordpress.com/2017/05/27/voxel-automata-terrain/
    // https://bitbucket.org/BWerness/voxel-automata-terrain/src/master/
  {
	static ImVec4 color0 = ImVec4(0,0,0,0);
	static ImVec4 color1 = ImVec4(210.0/255.0, 180.0/255.0, 140.0/255.0, 45.0/255.0);  // Wikipedia Tan
	static ImVec4 color2 = ImVec4(143.0/255.0, 151.0/255.0, 121.0/255.0, 95.0/255.0); // Wikipedia Artichoke Green

	static float lambda = 0.35;
	static float beta = 0.5;
	static float mag = 0.0;

	static int initmode;
	static float flip;

	static char str0[256] = "";

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,625));
    ImGui::Begin("Game of Life Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    //config options for this operation
	ImGui::Text("This is an interesting way to");
	ImGui::Text("generate shapes, developed by");
	ImGui::Text("Brent Werness - enter a rule, r");
	ImGui::Text("for Random or i for IsingRandom");	// may want to parameterize this further, with beta, lambda, mag - just want to get it working first
	ImGui::Text(" ");

	// string entry, letting the user input a rule
	ImGui::Text("Enter base62 encoded rule, r or i");
	ImGui::InputTextWithHint("", "", str0, IM_ARRAYSIZE(str0));

	// flip slider (float)
	ImGui::Text("Make nonzero for stochastic result");
    ImGui::SliderFloat(" flip", &flip, 0.0f, 1.0f, "%.3f");
	ImGui::Text(" ");

	ImGui::Text("Lambda is a parameter for Random");
	ImGui::SliderFloat(" lambda", &lambda, 0.0f, 1.0f, "%.3f");
	ImGui::Text(" ");

	ImGui::Text("Beta and Mag are parameters");
	ImGui::Text("for IsingRandom");
	ImGui::SliderFloat(" beta", &beta, 0.0f, 1.0f, "%.3f");
	ImGui::SliderFloat(" mag", &mag, 0.0f, 1.0f, "%.3f");
	ImGui::Text(" ");


	// mode slider (int)
	ImGui::Text("0 - fill side with 0");
	ImGui::Text("1 - fill side with 1");
	ImGui::Text("2 - fill side with 2");
	ImGui::Text("3 - fill side with random values");
    ImGui::SliderInt(" mode", &initmode, 0, 3);
	ImGui::Text(" ");

	// three colors
    ImGui::ColorEdit4(" State 0", (float*)&color0, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
	ImGui::ColorEdit4(" State 1", (float*)&color1, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
    ImGui::ColorEdit4(" State 2", (float*)&color2, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Compute", ImVec2(100, 22)))
    {
        // invoke the constructor, etc - return a string from the OpenGL_container::vat(...), and put it in str0
		glm::vec4 col0, col1, col2;
		col0 = glm::vec4(color0.x, color0.y, color0.z, color0.w);
		col1 = glm::vec4(color1.x, color1.y, color1.z, color1.w);
		col2 = glm::vec4(color2.x, color2.y, color2.z, color2.w);

		std::string temp = GPU_Data.vat(flip, std::string(str0), initmode, col0, col1, col2, lambda, beta, mag); 	//assign with the function call

		strcpy(str0, temp.c_str());	// you get to see how the random rule you generated, or retain the rule you entered
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = CA_MENU;

    goto done;
  }



  load_save_config_label:
    //lets you enter filenames and load/save from/to file
  {
    static char str0[256] = "";

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,385));
    ImGui::Begin("Load/Save Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    const char* listbox_items[LISTBOX_SIZE];

    //count up the number of non-null c-strings
    unsigned int i;
    for(i = 0; i < LISTBOX_SIZE && i < directory_strings.size(); ++i)
    {
        listbox_items[i] = directory_strings[i].c_str();
    }


    ImGui::Text("This function lets you load or");
    ImGui::Text("save of blocks using the PNG ");
    ImGui::Text("image format. ");
    ImGui::Text(" ");


    ImGui::Text("Files in saves folder:");
    static int listbox_select_index = 1;
    ImGui::ListBox(" ", &listbox_select_index, listbox_items, i, 10);



    ImGui::Text("Enter filename to save:");
    ImGui::InputTextWithHint(".png", "", str0, IM_ARRAYSIZE(str0));
    ImGui::SameLine();
    HelpMarker("USER:\nHold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.");


    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Load", ImVec2(60, 22)))
    {
        //load that image
        GPU_Data.load(directory_strings[listbox_select_index]);
    }

    ImGui::SameLine();

    if (ImGui::Button("Save", ImVec2(60, 22)))
    {
        if(hasPNG(std::string(str0)))
        {
            GPU_Data.save(std::string(str0));
        }
        else
        {
            GPU_Data.save(std::string(str0)+std::string(".png"));
        }

        update_listbox_items();
    }
    ImGui::SameLine();

    // ImGui::SetCursorPosX(140);
    if (ImGui::Button("Back", ImVec2(90, 22)))
      current_menu_state = UTIL_MENU;

    goto done;
  }

  shift_config_label:
  {

    static int xmove;
    static int ymove;
    static int zmove;
    static bool loop;
    static int shift_mode = 1;

    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,445));
    ImGui::Begin("Shift Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)

    ImGui::Text("This allows you to shift the voxel");
    ImGui::Text("data, by some amount along the x, ");
    ImGui::Text("y and z axes. Modes are described ");
    ImGui::Text("below. ");
    ImGui::Text(" ");
    ImGui::Text("If you turn on looping, data that ");
    ImGui::Text("goes off one side will appear on ");
    ImGui::Text("the opposite edge, torus-style.");


    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt(" x", &xmove, -DIM, DIM);
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt(" y", &ymove, -DIM, DIM);
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt(" z", &zmove, -DIM, DIM);
    ImGui::Text(" ");

    ImGui::SetCursorPosX(16);
    ImGui::SliderInt(" Mode", &shift_mode, 1, 3);
    ImGui::Text(" ");

    ImGui::SetCursorPosX(16);

    switch(shift_mode)
    {
        case 1:
            ImGui::Text("Mode 1: Ignore mask buffer, \nmove color data only");
            break;
        case 2:
            ImGui::Text("Mode 2: Respect mask buffer, \ncells retain color if masked");
            break;
        case 3:
            ImGui::Text("Mode 3: Carry mask buffer, \nmask and color move together");
            break;
        default:
            ImGui::Text("Pick a valid mode");
            break;
    }

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);
    ImGui::Checkbox(" loop", &loop);

    ImGui::Text(" ");
    ImGui::SetCursorPosX(16);

    if (ImGui::Button("Shift", ImVec2(90,22)))
        GPU_Data.shift(glm::ivec3(xmove,ymove,zmove), loop, shift_mode);

    ImGui::SameLine();
    ImGui::SetCursorPosX(140);

    if (ImGui::Button("Back", ImVec2(90, 22)))
      current_menu_state = UTIL_MENU;

    goto done;
  }

  limiter_config_label:
    //establishes range for r/g/b/a as well as lighting
  {
    static bool use_r;
    static bool use_g;
    static bool use_b;
    static bool use_a;

    static ImVec4 select_color;

    static float r_variance=0.1;
    static float g_variance=0.2;
    static float b_variance=0.3;
    static float a_variance=0.4;


    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,500));
    ImGui::Begin("Limiter Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked, or NULL to have no close button)



    ImGui::ColorEdit4("  Color", (float*)&select_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

    //sliders

    ImGui::Checkbox("use r", &use_r);
    ImGui::SameLine();
    ImGui::SliderFloat("r variance", &r_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use g", &use_g);
    ImGui::SameLine();
    ImGui::SliderFloat("g variance", &g_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use b", &use_b);
    ImGui::SameLine();
    ImGui::SliderFloat("b variance", &b_variance, 0.0f, 1.0f, "%.3f");

    ImGui::Checkbox("use a", &use_a);
    ImGui::SameLine();
    ImGui::SliderFloat("a variance", &a_variance, 0.0f, 1.0f, "%.3f");


    if (ImGui::Button("Mask", ImVec2(100, 22)))
    {
        //GPU_Data.mask_by_color(use_r, use_g, use_b, use_a, glm::vec4(select_color.x, select_color.y, select_color.z, select_color.w), r_variance, g_variance, b_variance, a_variance);
    }

    ImGui::SameLine();

    if (ImGui::Button("Back", ImVec2(100, 22)))
      current_menu_state = MASK_MENU;


    goto done;
  }

  done:
    //finished the label for the current window, so do the end() thing and render it


  ImGui::End();
  ImGui::Render();


  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());   //put imgui data into the framebuffer

  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  // cout << "viewport established with width: " << (int)io.DisplaySize.x << " and height: " << (int)io.DisplaySize.y << endl;


  //do my OpenGL raycasting here, over the cleared background



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

    if ((event.type == SDL_KEYUP  && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_X1)) //x1 is browser back on the mouse
    {
      switch (current_menu_state)
      {
        case MAIN_MENU:
          current_menu_state = EXIT;
          break;

        case DRAW_MENU:               //one level deep
        case MASK_MENU:
        case LIGHT_MENU:
        case CA_MENU:
        case UTIL_MENU:
          current_menu_state = MAIN_MENU;
          break;

        case SPHERE_CONFIG:           //two levels deep
        case PERLIN_NOISE_CONFIG:
        case GRID_CONFIG:
        case TRIANGLE_CONFIG:
        case ELLIPSOID_CONFIG:
        case CYLINDER_CONFIG:
        case TUBE_CONFIG:
        case CUBOID_CONFIG:
        case AABB_CONFIG:
        case HEIGHTMAP_CONFIG:
        case BLUR_CONFIG:
        case CLEAR_ALL_CONFIG:
          current_menu_state = DRAW_MENU;
          break;

          //mask config submenus
        case UNMASK_ALL_CONFIG:
        case TOGGLE_MASK_CONFIG:
        case MASK_BY_COLOR_CONFIG:
          current_menu_state = MASK_MENU;
          break;

          //cellular automata submenus
        case GAME_OF_LIFE_CONFIG:
        case WIREWORLD_CONFIG:
        case CA_TERRAIN_CONFIG:
          current_menu_state = CA_MENU;
          break;

          //utility submenus
        case LOAD_SAVE_CONFIG:
        case SHIFT_CONFIG:
        case LIMITER_CONFIG:
          current_menu_state = UTIL_MENU;
          break;

        default:
          current_menu_state = EXIT;
          break;
      }
    }
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_UP)
      GPU_Data.phi  += 0.03f;     //increment phi
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_DOWN)
      GPU_Data.phi  -= 0.03f;     //decrement phi
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_LEFT)
      GPU_Data.theta += 0.03f;    //increment theta
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_RIGHT)
      GPU_Data.theta -= 0.03f;    //decrement theta
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_MINUS)
      GPU_Data.scale += 0.1f;     //make scale smaller (offsets are larger)
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_EQUALS) //SDLK_PLUS requires that you hit the shift
      GPU_Data.scale -= 0.1f;     //make scale larger  (offsets are smaller)


    constexpr double pi = 3.14159265358979323846;

    // specific directions
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_F1)
      GPU_Data.theta = 0.0;
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_F2)
      GPU_Data.theta = pi/2.0;
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_F3)
      GPU_Data.theta = 3.0*(pi/2.0);
    if(event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_F4)
      GPU_Data.theta = pi;


    if(event.type == SDL_MOUSEWHEEL)  //allow scroll to do the same thing as +/-
    {
      if(event.wheel.y > 0) // scroll up
      {
        GPU_Data.scale -= 0.1f;
      }
      else if(event.wheel.y < 0) // scroll down
      {
        GPU_Data.scale += 0.1f;
      }
    }
  }
}

void voraldo::gl_data_setup()
{
  //set up the textures and stuff
  cout << "starting init..." << endl;
  GPU_Data.init();
  GPU_Data.load_textures();
  cout << "finished init." << endl;

  SDL_ShowWindow(window);
  //SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(DISPLAY), SDL_WINDOWPOS_CENTERED_DISPLAY(DISPLAY));
}

void voraldo::quit()
{
  //blow away all the OpenGL data
    //todo


  //shutdown everything
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  //fullscreen exit splash? maybe if it's a quick one - I really liked that glitchy blinking eye gif

  //destroy window
  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  SDL_Delay(100);
  cout << endl << "GOODBYE" << endl; //last line of code before main's return statement
  SDL_Delay(100);
}
