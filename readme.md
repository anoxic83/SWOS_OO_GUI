# SWOS Overlay/Override GUI plugin
---
Autor: AnoXic
Plugin for SWOS to overlay or override SWOS Menu with ImGUI.
Support both SWOS rendering method (SDLRenderer/OpenGL).

- Overlay Mode 
Install wrappers and draw imgui window overlay SWOS.

- Override Mode
Install wrapper on SWOS menu function and override.

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

#Important!
Replace orginal SDL.dll ( in this plugin used version 2.0.22 ), imgui sdlrenderer needs newer version.

# Requirements
All requirements copy to deps folder
SDL2
Glew
ImGui
stb - for stb_image.h

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

---
# Release
Unpack 7z file and copy all files from folder swos to directory where is swos-port-Release.exe file.


# Changes Log
**v0.2.2b**
- Update to SWOS version 4.1.4
- Add Memory Hex Editor (autor: (c) 2017-2019 Omar Cornut) in overlay mode
**v0.2.1b**
- Update to SWOS version 4.1.2
**v0.2.0b**
- Initial github release, for SWOS version 4.1.1