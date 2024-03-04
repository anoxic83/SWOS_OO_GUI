# SWOS Overlay/Override GUI plugin
---
Autor: AnoXic
Plugin for SWOS to overlay or override SWOS Menu with ImGUI.
Support both SWOS rendering method (SDLRenderer/OpenGL).

### Donations

Support the author of the project with a donation:

[![paypal](https://www.paypalobjects.com/en_US/PL/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?cmd=_s-xclick&hosted_button_id=P8MAW3UMJWGRC)


> ### Overlay Mode

> Install wrappers and draw imgui window overlay SWOS.

> Hex-editor is enabled. 

> In career mode, goto buttons to begin career, bank balance and own team is supported.

> If OpenGL mode in SWOS enabled, memory-hex-editor window can be dragged off screen

> ### Override Mode

> Install wrapper on SWOS menu function and override.


Both method not change anythink in original SWOS.
All original SWOS function will be executed (except MenuProc if override).

Change version in anxmain.cc variable bool overlay = (true/false).

---
# SWOS-Win32-ASI
SWOS-Win32-ASI is a framework to create patches to **Sensible World of Soccer** version Windows 32-bit port.
Plugins swaps winmm.dll and load all files with .ASI extension from swos/plugins folder.
.ASI files are a windows .dll files.

----
# Compilation
On this moment only support MSVC compliler (winmm.dll). 
Suggested method use Visual Studio Code:
Download CMake Tools extension.
Open this directory as Open Folder, select kit as Visual Studio x86 and complile.

# Requirements
All requirements copy to deps folder
SDL2 - https://github.com/libsdl-org/SDL
Glew - https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
ImGui - https://github.com/ocornut/imgui
imgui_memory_editor - https://github.com/ocornut/imgui_club/tree/main
stb - https://github.com/nothings/stb

# Files
include/winmm/winmmmain.h - header of wrapper to winmm.dll (Loaded orginaly by SWOS)
include/winmm/winmmmain.def - definition to windows dll file (exports)
include/swosasi/swoshook.h,
src/swosasi/swoshook.cc - support functions to create patches and finding pattern in memory.

src/anxswos/anxmain.cc - main dll with wrappers to init SWOS and main class
include/anxswos/anxswos.h,
src/anxswos/anxswos.cc - main class to drawing imgui on SWOS
include/anxswos/anxtex.h,
src/anxswos/anxtex.cc, - helper class to drawing texture for OpenGL mode

# Installation
copy compiled file winmm.dll (from bin folder) to main swos folder (where is swos-port-Release.exe),
copy plugins folder to this same directory or create directory plugins and copy file swscc.asi

> [!IMPORTANT]
> Replace orginal SDL.dll ( in this plugin used version 2.0.22 ), imgui sdlrenderer needs newer version.
---
# Release
Unpack 7z file and copy all files from folder swos to directory where is swos-port-Release.exe file.

# Screenshots
![ovelay_hex1](https://user-images.githubusercontent.com/24848605/183255400-224f2be8-ae31-440b-b645-8e9bf67f6ef9.png)
![ovelay_hex2](https://user-images.githubusercontent.com/24848605/183255402-d0149ca5-3cce-460f-9b8a-0dce109e28e9.png)
![overide](https://user-images.githubusercontent.com/24848605/183255403-ec658e71-731b-4481-aa7f-55f936b2f4d3.png)

# Changes Log
**v0.2.6b**
- Update to SWOS version 6.3.7

**v0.2.5b**
- Update to SWOS version 6.3.0

**v0.2.4b**
- Enable off-swos-screen display (OpenGL mode)
- Update deps:
- SDL2 -> 2.3.0
- ImGui -> 1.90.3 (docking)

**v0.2.3b**
- Update to SWOS version 6.2.3
  
**v0.2.2b**
- Update to SWOS version 4.1.4
- Add Memory Hex Editor (autor: (c) 2017-2019 Omar Cornut) in overlay mode
  
**v0.2.1b**
- Update to SWOS version 4.1.2
  
**v0.2.0b**
- Initial github release, for SWOS version 4.1.1
