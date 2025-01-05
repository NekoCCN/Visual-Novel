
set(Render_SRC
  "MainMenu/MainMenu.h"
  "MainMenu/MainMenu.cc" 
  "BasicRender/BasicRender.h"
)

add_library(VisualNovelRender STATIC ${Render_SRC})


set(Render_Require
  SDL3-static
  SDL3_image::SDL3_image
  imgui
  VisualNovelCore
  VisualNovelAsset
)

target_link_libraries(VisualNovelRender PUBLIC ${Render_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovelCore PROPERTY CXX_STANDARD 20)
endif()