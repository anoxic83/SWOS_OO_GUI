#pragma once
/*

MIT License

Copyright (c) 2021 Anoxic

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <windows.h>
#include <DbgHelp.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <functional>

struct SWOSRegisters
{
  uint32_t D[8];
  uint32_t A[7];
};

class SWOSHook
{
private:
  static uintptr_t m_BaseAddress;
  static uintptr_t m_CompetitionTablePtr;
  static uintptr_t m_DSegPtr;
public:
  static void Init();
  static uintptr_t GetBaseAddress();
  //* Get address of League/Cup Competition pointers
  static uintptr_t GetCompetitionTablePtr();
  //* Get start address of DSeg (DOS4GW original SWOS data segment)
  static uintptr_t GetDSegDataPtr();
  //* Get Access to Sensible World of Soccer internal registers (Data [8 * 32bit] and Address [7 * 32bit] <Virtual M68k regs>)
  static SWOSRegisters* GetSWOSRegisters();
  static uintptr_t FindInMemory(void* data, size_t sizeofdata);
  static bool WriteMemory(uintptr_t address, void* value, size_t size);
  static bool ReadMemory(uintptr_t address, void* value, size_t size);
  static bool SetMemory(uintptr_t address, uint8_t value, size_t size);
  static bool MakeCall(uintptr_t address, void* func);
  template<typename Ret>
  static Ret CallSWOS(uintptr_t address)
  {
    return ((Ret (__cdecl *)(void))address)();
  }
};