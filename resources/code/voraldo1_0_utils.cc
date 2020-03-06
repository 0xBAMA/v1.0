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

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   //really excited by the fact imgui has hsv selectors that let you change this easily from inside the program
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( window );

  cout << " done." << endl;

}

void voraldo::draw_menu_and_take_input()
{

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

  ImGuiIO& io = ImGui::GetIO();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  // static bool show_another_window = true;
  // static bool show_demo_window = true;
  static bool show_my_window = true;

  // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  // if (show_demo_window)
  //     ImGui::ShowDemoWindow(&show_demo_window);


  // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
  // {
  //     static float f = 0.0f;
  //     static int counter = 0;
  //
  //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
  //
  //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
  //     ImGui::Checkbox("Another Window", &show_another_window);
  //     ImGui::Checkbox("My Window", &show_my_window);
  //
  //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
  //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
  //
  //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
  //         counter++;
  //
  //     ImGui::SameLine();
  //     ImGui::Text("counter = %d", counter);
  //
  //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  //     ImGui::End();
  // }


  // // 3. Show another simple window.
  // if (show_another_window)
  // {
  //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
  //     ImGui::Text("Hello from another window!");
  //     if (ImGui::Button("Close Me"))
  //         show_another_window = false;
  //     ImGui::End();
  // }

  if(show_my_window)
  {
    ImGui::SetNextWindowSize(ImVec2(220,190));
    ImGui::Begin("My Window", &show_my_window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)


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

    ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }



  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);

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
