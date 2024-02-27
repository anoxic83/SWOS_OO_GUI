#pragma once
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include <SDL2/SDL_opengl.h>
#include "swoshook.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_memory_editor.h"
#include "anxtex.h"

class AnxSWOS
{
private:
  uintptr_t m_Base;
  uintptr_t m_DSeg;
  SDL_Window* m_Window;
  SDL_GLContext m_GLContext;
  SDL_Renderer* m_Renderer;
  ImGuiContext* m_ImGuiCtx;
  Texture* m_Background;
  SDL_Texture* m_BackgroundSDL;
  bool m_GUIOverlay;
  bool m_OpenGLRenderer;
  bool IsCareer();
  bool m_HexMemory;
  MemoryEditor m_HexEdit;
  uintptr_t m_CurrentHexAddress;
  size_t m_DataSize;
  SDL_Texture* loadSDLTexture(const char* path);
public:
  AnxSWOS(uintptr_t base, bool overlay);
  virtual ~AnxSWOS();
  void Init();
  void OnEvent(SDL_Event* e);
  void Draw();
};