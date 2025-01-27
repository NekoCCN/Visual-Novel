
set(Resolution_SRC
  "FormatChecker/FormatChecker.h"
  "NormalLineResolution/NormalLineResolution.h"
  "NormalLineResolution/NormalLineResolution.cc"
  "DefinedPart/DefinePart.h"
  "DefinedPart/DefinePart.cc"
  "CommandFactory/CommandFactory.h"
  "EntryPoint.h"
  "EntryPoint.cc")

add_library(VisualNovelResolution STATIC ${Resolution_SRC})

set(Resolution_Require
  SDL3-static
  SDL3_image::SDL3_image
  imgui
  VisualNovelCore
  VisualNovelAsset
  VisualNovelRender
)

target_link_libraries(VisualNovelResolution PUBLIC ${Resolution_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovelResolution PROPERTY CXX_STANDARD 20)
endif()