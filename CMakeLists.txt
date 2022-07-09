cmake_minimum_required(VERSION 3.1)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})

project(ANMSWOS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 99)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  MESSAGE(FATAL_ERROR "64 compiling not allowed!")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  MESSAGE(FATAL_ERROR "GCC compiling not allowed!")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

set(ROOTSRC ${PROJECT_SOURCE_DIR}/src)
set(ROOTINC ${PROJECT_SOURCE_DIR}/include)

set(SWSASIINC ${PROJECT_SOURCE_DIR}/include/swosasi)
set(SWSASISRC ${PROJECT_SOURCE_DIR}/src/swosasi)
set(WINMMSRC ${PROJECT_SOURCE_DIR}/src/winmm)
set(WINMMINC ${PROJECT_SOURCE_DIR}/include/winmm)

set(ANXSWSSRC ${PROJECT_SOURCE_DIR}/src/anxswos)
set(ANXSWSINC ${PROJECT_SOURCE_DIR}/include/anxswos)


set(DEPS ${PROJECT_SOURCE_DIR}/deps)

set(SDL2INC ${DEPS}/SDL2/include)
set(SDL2LIB ${DEPS}/SDL2/lib/x86)

set(GLEWINC ${DEPS}/GLEW/include)
set(GLEWLIB ${DEPS}/GLEW/lib/Release/x86)

set(IMGUILIB ${DEPS}/ImGui)
set(STBLIB ${DEPS}/stb)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -masm=intel")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -s -DNDEBUG")
  set(CMAKE_CXx_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /O1 /Ob1 /DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /DNDEBUG")
endif()


include_directories(${WINMMSRC})
include_directories(${WINMMINC})


include_directories(${SWSASIINC})
include_directories(${SWSASISRC})

include_directories(${ANXSWSSRC})
include_directories(${ANXSWSINC})


include_directories(${SDL2INC})
link_directories(${SDL2LIB})

include_directories(${GLEWINC})
link_directories(${GLEWLIB})

include_directories(${IMGUILIB})
include_directories(${IMGUILIB}/backends)
include_directories(${IMGUILIB}/misc/cpp)

include_directories(${STBLIB})


set(CMAKE_SHARED_LIBRARY_PREFIX "")
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  #WINMM WRAPPER
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

  set(WINMM_FILES
    ${WINMMSRC}/winmmmain.cc
    ${WINMMINC}/winmmmain.def
  )

add_library(winmm SHARED ${WINMM_FILES})
endif()

#ASI PLUGINS
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

set(ANMSWOS_FILES
  ${SWSASISRC}/swoshook.cc
  ${ANXSWSSRC}/anxmain.cc
  ${ANXSWSSRC}/anxswos.cc
  ${ANXSWSSRC}/anxtex.cc
  ${IMGUILIB}/imgui.cpp
  ${IMGUILIB}/imgui_widgets.cpp
  ${IMGUILIB}/imgui_draw.cpp
  ${IMGUILIB}/imgui_tables.cpp
  ${IMGUILIB}/imgui_demo.cpp
  ${IMGUILIB}/misc/cpp/imgui_stdlib.cpp
  ${IMGUILIB}/backends/imgui_impl_sdl.cpp
  ${IMGUILIB}/backends/imgui_impl_sdlrenderer.cpp
  ${IMGUILIB}/backends/imgui_impl_opengl3.cpp
)

add_library(swos_oo_gui SHARED ${ANMSWOS_FILES})
target_link_libraries(swos_oo_gui SDL2 OpenGL32)
target_compile_definitions(swos_oo_gui PUBLIC MAKEDLL)
set_target_properties(swos_oo_gui PROPERTIES SUFFIX ".asi")


