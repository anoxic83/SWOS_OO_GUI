#include <Windows.h>
#include "swoshook.h"
#include <SDL2/SDL.h>
#include "anxswos.h"
#include <cstdio>

// Wrap call init
const uintptr_t ptrInitSWOS = 0x485F20 - 0x400000;
const uintptr_t ptrInitFunction = 0x484290 - 0x400000;

// Wrap call processEvent(ecx = const &SDL_Event);
const uintptr_t ptrPollEvent = 0x4540D9 - 0x400000;
const uintptr_t ptrProcessEvent = 0x453D10 - 0x400000;

// OVELAY MENU DRAW WRAPPERS
// Wrap OpenGL amiga
const uintptr_t ptrSwapBufferAmiga = 0x4F43BC - 0x400000;
// Wrap OpenGL wide
const uintptr_t ptrSwapBufferWide = 0x4F48A2 - 0x400000;
// Wrap Renderer
const uintptr_t ptrRenderPresent = 0x4F3ED8 - 0x400000;

// OVERWRITE MENU DRAW
const uintptr_t ptrMenuProc = 0x487159 - 0x400000;



uintptr_t base = 0;
AnxSWOS* awos = NULL;

void InitWrapper()
{
  awos->Init();
  SWOSHook::CallSWOS<void>(ptrInitFunction + base);
}

void PollEventWrapper()
{
  SDL_Event* e;
  __asm
  {
    mov e, ecx
    mov eax, ptrProcessEvent
    add eax, base
    call eax
  }
  awos->OnEvent(e);
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
    bool overlay = true;
    awos = new AnxSWOS(base, overlay);
    SWOSHook::MakeCall(ptrPollEvent + base, PollEventWrapper);
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