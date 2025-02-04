
set(Render_SRC
  "MainMenu/MainMenu.h"
  "MainMenu/MainMenu.cc" 
  "BasicRender/BasicRender.h"
  "FileWeiget/ImGuiFileDialog/ImGuiFileDialog.cpp"
  "FileWeiget/ChooseFile.h"
  "FileWeiget/ChooseVnap.h"
  "LogWindow/LogWindow.h"
  "BasicTexture/BasicTexture.h"
  "FontCore/FontCore.h"
  "FontCore/FontCore.cc"
)

add_library(VisualNovelRender STATIC ${Render_SRC})

set(Render_Require
  SDL3-static
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  imgui
  VisualNovelCore
)

target_link_libraries(VisualNovelRender PUBLIC ${Render_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovelRender PROPERTY CXX_STANDARD 20)
endif()