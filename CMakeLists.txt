cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Visual-Novel")

# Visual Text SRC
include_directories( ./ )
add_subdirectory ("Core")
add_subdirectory ("Asset")
add_subdirectory ("Render")
add_subdirectory ("Entry")
add_subdirectory ("Resolution")

# SDL
set (BUILD_SHARED_LIBS false)
set(SDL_STATIC on)
add_subdirectory( External/SDL )
include_directories( External/SDL/include )

# SDL Extension
add_subdirectory( External/SDL_image )
include_directories( External/SDL_image/include )
add_subdirectory( External/SDL_ttf )
include_directories( External/SDL_ttf/include )

# ImgUI
add_subdirectory( External/prev_imgui )
include_directories( External/prev_imgui )