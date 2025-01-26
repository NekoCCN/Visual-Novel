
set(Core_SRC
  "Exception/Exception.cc"
  "Exception/Exception.h"
  "Window/Window.h"
  "Window/Window.cc" 
  "Monitor/Monitor.h"
  "Monitor/Monitor.cc"
  "Transform/Transform.h" 
  "Transform/Transform.cc" 
  "CommandList/CommandList.h"
  "LogSystem/LogFunction.h"
  "LogSystem/LogFunction.cc"
  "CoordinateSystem/RatioCoordinateSystem.h"
  "GuiContext/GuiContext.h" 
  "SpscQueue/SpscQueue.h"
 "TSVector/TSVector.h")

add_library(VisualNovelCore STATIC ${Core_SRC})


set(Core_Require
  SDL3-static
  imgui
)

target_link_libraries(VisualNovelCore PUBLIC ${Core_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovelCore PROPERTY CXX_STANDARD 20)
endif()