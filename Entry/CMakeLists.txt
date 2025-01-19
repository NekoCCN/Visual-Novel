cmake_minimum_required (VERSION 3.8)

add_executable (VisualNovel "Application.cc")

set (VisualNovelRequire
  SDL3_ttf::SDL3_ttf
  VisualNovelCore
  VisualNovelAsset
  VisualNovelRender
  VisualNovelResolution
)

target_link_libraries (VisualNovel ${VisualNovelRequire})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovel PROPERTY CXX_STANDARD 20)
endif()
