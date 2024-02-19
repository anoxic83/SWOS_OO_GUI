#include "winmmmain.h"
#include <cstdio>
HMODULE hm;

std::wstring SHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken)
{
	std::wstring r;
	WCHAR* szSystemPath = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(rfid, dwFlags, hToken, &szSystemPath)))
	{
		r = szSystemPath;
	}
	CoTaskMemFree(szSystemPath);
	return r;
};

static LONG OriginalLibraryLoaded = 0;
void LoadOriginalLibrary()
{
	auto szSystemPath = SHGetKnownFolderPath(FOLDERID_System, 0, nullptr) + L'\\' + L"winmm.dll";
	winmm.LoadOriginalLibrary(LoadLibraryW((LPCWSTR)szSystemPath.c_str()));
}

void LoadPlugin()
{
	printf("[WINMM.DLL]:: Loading Plugin...\n");
	auto CurrentDir = std::filesystem::current_path();
	std::filesystem::path ScriptPath("plugins");
	auto PluginDir = CurrentDir / ScriptPath;
	for (const auto & entry : std::filesystem::directory_iterator(PluginDir))
	{
		if (entry.path().extension() == ".asi")
		{
			HMODULE hpath = LoadLibraryW((LPCWSTR)entry.path().generic_wstring().c_str());

		}
	}
}

void Init()
{
	LoadOriginalLibrary();
	auto hwSWOS = (uintptr_t)GetModuleHandle(NULL);
	IMAGE_DOS_HEADER*  dos = (IMAGE_DOS_HEADER*)(hwSWOS);
	IMAGE_NT_HEADERS*  nt = (IMAGE_NT_HEADERS*)(hwSWOS + dos->e_lfanew);
	if (nt->OptionalHeader.AddressOfEntryPoint != 0x01475A1)		// Check the swos-port-release executable version
	{
    if (MessageBoxA(NULL, "Not valid version of SWOS executable. Supported version: 6.2.3\nContinue anyway?", "ASI Loader v0.4.0b", MB_YESNO + MB_ICONASTERISK) == IDYES)
    {
		  LoadPlugin();
    }
	  else
	  {
		  MessageBoxA(NULL, "Unsupported version of SWOS, or not SWOS-Win32-Port.", "ASI Loader v0.4.0b", MB_ICONWARNING + MB_OK);
	  }
  }
  else
  {
    LoadPlugin();
  }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		hm = hModule;
		Init();
	}
	return TRUE;
}