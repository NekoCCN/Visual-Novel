
set(Asset_SRC
  "ThemePack/ThemePack.h"
  "ThemePack/ThemePack.cc"
  "ThemeAssetList/ThemeAssetList.h"
  "ThemePackCreator/ThemePackCreator.h"
  "ThemePackCreator/ThemePackCreator.cc"
  "AssetPackWStream/AssetPackWStream.h"
  "AssetPackWStream/AssetPackWStream.cc"
 "ConfigBinaryPack/ConfigBinaryPack.h")

add_library(VisualNovelAsset STATIC ${Asset_SRC})


set(Asset_Require
  VisualNovelCore
  SDL3-static
)

target_link_libraries(VisualNovelAsset PUBLIC ${Asset_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualNovelCore PROPERTY CXX_STANDARD 20)
endif()