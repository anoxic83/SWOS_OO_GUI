#include "anxswos.h"
#define SWSLOG_IMPLEMENTATION
#include "swslog.h"
//Init vars
const uintptr_t ptrEnhancement = 0x9DAF94 - 0x400000;
const uintptr_t ptrSDLWindow = 0x4B79A6C - 0x400000;
const uintptr_t ptrSDLRenderer = 0x505940C	 - 0x400000;
const uintptr_t ptrGLContext = 0x9DB0F0 - 0x400000;

const uintptr_t ptrWindowWidth = 0x4E76730 - 0x400000;
const uintptr_t ptrWindowHeight = 0x4E76734 - 0x400000;

// Helpers
const uintptr_t ptrInputingText = 0x54FDA73 - 0x400000;

// Career 
const uintptr_t ptrGameType = 0x5469078 - 0x400000;

AnxSWOS::AnxSWOS(uintptr_t base, bool overlay)
: m_Base(base), m_Window(nullptr), m_Renderer(nullptr), m_ImGuiCtx(nullptr), m_GUIOverlay(overlay)
{
  log_init(LOG_LEV_INFO, "plugins/swos_oo_gui.log");
  log_info("[ANXSWOS] => Plugin loaded.");
  log_info("[ANXSWOS] => SWOS Base address: 0x%p", m_Base);
  log_info("[ANXSWOS] => Plugin mode: %s", (m_GUIOverlay) ? "Overlay" : "Override");
}

AnxSWOS::~AnxSWOS()
{
  if (m_OpenGLRenderer)
    ImGui_ImplOpenGL3_Shutdown();
  else
    ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  log_info("[ANXSWOS] => Plugin unloaded.");
}

SDL_Texture* AnxSWOS::loadSDLTexture(const char* path)
{
  // Copy from https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = SDL_LoadBMP( path );
  if( loadedSurface == NULL )
  {
    log_error( "[AnxSWOS] => Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError() );
  }
  else
  {
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( m_Renderer, loadedSurface );
    if( newTexture == NULL )
    {
        log_error( "[AnxSWOS] => Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
    }
    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}

bool AnxSWOS::IsCareer()
{
  int16_t gameType = 0;
  SWOSHook::ReadMemory(ptrGameType + m_Base, &gameType, 2);
  return (gameType == 4);
}

void AnxSWOS::Init()
{
  SWOSHook::ReadMemory(ptrEnhancement + m_Base, &m_OpenGLRenderer, 4);
  uint32_t wnd = 0;
  SWOSHook::ReadMemory(ptrSDLWindow + m_Base, &wnd, 4);
  m_Window = (SDL_Window*)wnd;
  if (m_OpenGLRenderer)
  {
    SWOSHook::ReadMemory(ptrGLContext + m_Base, &m_GLContext, 4);
  }
  else
  {
    uint32_t ren = 0;
    SWOSHook::ReadMemory(ptrSDLRenderer + m_Base, &ren, 4);
    m_Renderer = (SDL_Renderer*)ren;
  }
  IMGUI_CHECKVERSION();
  m_ImGuiCtx = ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

  ImGui::StyleColorsDark();


  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  if (m_OpenGLRenderer)
  {
    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
  }
  else
  {
    ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer_Init(m_Renderer);
  }

  if (!m_GUIOverlay)
    if (m_OpenGLRenderer)
      m_Background = new Texture("swtitle-bg-amiga.bmp");
    else
      m_BackgroundSDL = loadSDLTexture("swtitle-bg-amiga.bmp");
}

void AnxSWOS::OnEvent(SDL_Event* e)
{
  if (m_ImGuiCtx != nullptr)
  {
    ImGui_ImplSDL2_ProcessEvent(e);
  }
}

void AnxSWOS::Draw()
{
  if (!m_GUIOverlay)
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT)
      {
        ExitProcess(0);
      }
    }
    if (m_OpenGLRenderer)
    {
      glClearColor(0, 0, 0.2f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
      SDL_SetRenderDrawColor(m_Renderer, 0, 0, 51, 255);
      SDL_RenderClear(m_Renderer);
    }
  }
  int width;
  int heigth;
  SDL_GetWindowSize(m_Window, &width, &heigth);
  
  if (m_OpenGLRenderer)
    ImGui_ImplOpenGL3_NewFrame();
  else
  {
    ImGui_ImplSDLRenderer_NewFrame();
  }
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  //DRAW
  if (!m_GUIOverlay)
    if (m_OpenGLRenderer)
      ImGui::GetBackgroundDrawList()->AddImage((void*)m_Background->GetID(), ImVec2(0, 0), ImVec2(width, heigth));
    else
      ImGui::GetBackgroundDrawList()->AddImage((void*)m_BackgroundSDL, ImVec2(0, 0), ImVec2(width, heigth));

  ImGui::SetNextWindowBgAlpha(0.4f);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(width * 0.2, heigth * 0.2), ImGuiCond_FirstUseEver);
  const std::string imwinname = (m_GUIOverlay) ? "GUI Plugin [OvelayMode]" : "GUI Plugin [Override Mode]";
  ImGui::Begin(imwinname.c_str());
  if (ImGui::CollapsingHeader("About"))
  {
    ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "AnxSWOS Overlay/Overwite v0.2.1b");
    
    ImGui::Text("(c)2022 AnoXic");
  }
  ImGui::End();

  ImGui::Render();
  if (m_OpenGLRenderer)
  {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_Window);
  }
  else
  {
    SDL_RenderSetScale(m_Renderer, 1, 1);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_Renderer);
  }
}