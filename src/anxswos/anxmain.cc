#include <Windows.h>
#include "swoshook.h"
#include <SDL2/SDL.h>
#include "anxswos.h"
#include <cstdio>


// Wrap call init (replace call from _SWOS_main)
const uintptr_t ptrInitSWOS = 0x4861AB - 0x400000;
const uintptr_t ptrInitFunction = 0x485DC0 - 0x400000;

const uintptr_t ptrPollEvent = 0x453FE0 - 0x400000;

// OVELAY MENU DRAW WRAPPERS
// Wrap OpenGL amiga => Swap call SDL_GL_SwapBuffers
const uintptr_t ptrSwapBufferAmiga = 0x4F5A6C - 0x400000;
// Wrap OpenGL wide
const uintptr_t ptrSwapBufferWide = 0x4F5F52 - 0x400000;
// Wrap Renderer => Swap SDL_RenderPresent
const uintptr_t ptrRenderPresent = 0x4F5588 - 0x400000;

// OVERWRITE MENU DRAW 
const uintptr_t ptrMenuProc = 0x4873E9 - 0x400000;



uintptr_t base = 0;
AnxSWOS* awos = NULL;

void InitWrapper()
{
  awos->Init();
  SWOSHook::CallSWOS<void>(ptrInitFunction + base);
}

int __cdecl PollEventWrapper(SDL_Event* event)
{
  int ret = SDL_PollEvent(event);
  awos->OnEvent(event);
  return ret;
}

void DrawWrapper()
{
  awos->Draw();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
    //MessageBoxA(nullptr, "Loaded CC", "SWSCC", MB_OK | MB_ICONEXCLAMATION);
    SWOSHook::Init();
    base = SWOSHook::GetBaseAddress();
    /* 
    ======================================
    HERE CHANGE VERSION OVERLAY TO OVERIDE
    */ 
    bool overlay = false;
    awos = new AnxSWOS(base, overlay);
    SWOSHook::MakeCall(ptrPollEvent + base, PollEventWrapper);
    SWOSHook::SetMemory(ptrPollEvent + base + 5, 0x90, 1);
    
    SWOSHook::MakeCall(ptrInitSWOS + base, InitWrapper);
    if (overlay)
    {
      SWOSHook::MakeCall(ptrSwapBufferAmiga + base, DrawWrapper);
      SWOSHook::SetMemory(ptrSwapBufferAmiga + base + 5, 0x90, 1);
      SWOSHook::MakeCall(ptrSwapBufferWide + base, DrawWrapper);
      SWOSHook::SetMemory(ptrSwapBufferWide + base + 5, 0x90, 1);
      SWOSHook::MakeCall(ptrRenderPresent + base, DrawWrapper);
      SWOSHook::SetMemory(ptrRenderPresent + base + 5, 0x90, 1);
    }
    else
    {
      SWOSHook::MakeCall(ptrMenuProc + base, DrawWrapper);
      //SWOSHook::SetMemory(ptrMenuDelay + base + 5, 0x90, 5);
    }
	}
  if (reason == DLL_PROCESS_DETACH)
  {
    delete awos;
  }
	return TRUE;
}