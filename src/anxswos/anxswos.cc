#include "anxswos.h"
#define SWSLOG_IMPLEMENTATION
#include "swslog.h"
//Init vars
const uintptr_t ptrEnhancement = 0xA57534 - 0x400000; // Is OpenGL or SDL
const uintptr_t ptrSDLWindow = 0x4BF60FC - 0x400000;
const uintptr_t ptrSDLRenderer = 0xA576D8 - 0x400000;
const uintptr_t ptrGLContext = 0xA576DC - 0x400000;

const uintptr_t ptrWindowWidth = 0x4EF7AE8 - 0x400000;
const uintptr_t ptrWindowHeight = 0x4EF7AEC - 0x400000;

// Helpers
const uintptr_t ptrInputingText = 0x54FDA73 - 0x400000;

// Career 
const uintptr_t ptrGameType = 0x54FC068 - 0x400000;
//54FC068
//old : 0x547B078 diff: 80FF0
const uintptr_t ptrCareerFileBuffer = 0x54645B6 - 0x400000 + 0x80FF0;
const uintptr_t ptrNewBalance = 0x5471B92 - 0x400000 + 0x80FF0;
const uintptr_t ptrCareerTeam = 0x5472136 - 0x400000 + 0x80FF0;

AnxSWOS::AnxSWOS(uintptr_t base, bool overlay)
: m_Base(base), m_Window(nullptr), m_Renderer(nullptr), m_ImGuiCtx(nullptr), m_GUIOverlay(overlay)
{
  log_init(LOG_LEV_INFO, "plugins/swos_oo_gui.log");
  log_info("[ANXSWOS] => Plugin loaded.");
  log_info("[ANXSWOS] => SWOS Base address: 0x%p", m_Base);
  log_info("[ANXSWOS] => Plugin mode: %s", (m_GUIOverlay) ? "Overlay" : "Override");
  m_CurrentHexAddress = m_Base;
  m_DataSize = 0xffff;
  m_HexMemory = false;
}

AnxSWOS::~AnxSWOS()
{
  if (m_OpenGLRenderer)
    ImGui_ImplOpenGL3_Shutdown();
  else
    ImGui_ImplSDLRenderer2_Shutdown();
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
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
    ImGui_ImplSDLRenderer2_Init(m_Renderer);
  }

  if (!m_GUIOverlay)
    if (m_OpenGLRenderer)
      m_Background = new Texture("swtitle-bg.bmp");
    else
      m_BackgroundSDL = loadSDLTexture("swtitle-bg.bmp");
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
    ImGui_ImplSDLRenderer2_NewFrame();
  }
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  //DRAW
  ImVec2 pos = ImGui::GetMainViewport()->Pos;
  ImVec2 size = ImGui::GetMainViewport()->Size;
  if (!m_GUIOverlay)
    if (m_OpenGLRenderer)
      ImGui::GetBackgroundDrawList()->AddImage((void*)m_Background->GetID(), pos, ImVec2(pos.x + width, pos.y + heigth));
    else
      ImGui::GetBackgroundDrawList()->AddImage((void*)m_BackgroundSDL, ImVec2(0, 0), ImVec2(width, heigth));

  ImGui::SetNextWindowBgAlpha(0.4f);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(width * 0.2f, heigth * 0.2f), ImGuiCond_FirstUseEver);
  const std::string imwinname = (m_GUIOverlay) ? "GUI Plugin [OvelayMode]" : "GUI Plugin [Override Mode]";
  ImGui::Begin(imwinname.c_str());
  if (m_GUIOverlay)
  {
    if (ImGui::CollapsingHeader("Hex Editor", ImGuiTreeNodeFlags_DefaultOpen))
    {
      ImGui::InputInt("Memory Address: ", (int*)&m_CurrentHexAddress, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
      ImGui::InputInt("Data Size: ", (int*)&m_DataSize, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

      if (IsCareer())
      {
        ImGui::Text("Career Begin Data <0x%p>:",  ptrCareerFileBuffer + m_Base); ImGui::SameLine();
        if (ImGui::Button("Show###BeginData"))
        {
          m_CurrentHexAddress = ptrCareerFileBuffer + m_Base;  
          m_HexMemory = true;
        }
        ImGui::Text("Career New Balance <0x%p>:",  ptrNewBalance + m_Base); ImGui::SameLine();
        if (ImGui::Button("Show###NewBalance"))
        {
          m_CurrentHexAddress = ptrNewBalance + m_Base;  
          m_HexMemory = true;
        }
        ImGui::Text("Career Team <0x%p>:",  ptrCareerTeam + m_Base); ImGui::SameLine();
        if (ImGui::Button("Show###CareerTeam"))
        {
          m_CurrentHexAddress = ptrCareerTeam + m_Base;  
          m_HexMemory = true;
        }
      }
      ImGui::Text("SWOS Base <0x%p>:", m_Base); ImGui::SameLine();
      if (ImGui::Button("Show###SWOSBase"))
      {
        m_CurrentHexAddress = m_Base;
        m_HexMemory = true;
      }
      ImGui::Separator();
      m_HexEdit.DrawContents((void*)m_CurrentHexAddress, m_DataSize);
    }
  }
  if (ImGui::CollapsingHeader("About", ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "AnxSWOS Overlay/Overwite v0.2.4b");
    ImGui::Text("Copyright (c)2022-2024 AnoXic");
    ImGui::Separator();
    ImGui::Text("HexMemory Editor created by: (c) 2017-2019 Omar Cornut");
    ImGui::Text("https://github.com/ocornut/imgui_club");
  }
  ImGui::End();


  // RENDER
  ImGui::Render();
  if (m_OpenGLRenderer)
  {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
    SDL_GL_SwapWindow(m_Window);
  }
  else
  {
    SDL_RenderSetScale(m_Renderer, 1, 1);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_Renderer);
  }
}