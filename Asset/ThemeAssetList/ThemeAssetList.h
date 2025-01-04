#pragma once
#ifndef VISUALNOVEL_ASSET_THEMEASSETLIST_H
#define VISUALNOVEL_ASSET_THEMEASSETLIST_H
#include <cstdint>

namespace vn
{
	namespace asset
	{
		namespace theme
		{
			class ThemeAssetList
			{
			public:
				constexpr static uint8_t theme_asset_list_size_ = 6;

				enum ThemeAssetListIndex :uint8_t
				{
					BASIC_FONT = 0,
					MAIN_MENU_LOGO = 1,
					CREATE_FILE = 2,
					LOAD_FILE = 3,
					SETTING = 4,
					CLOSE = 5,
				};

				static const char* to_string(const ThemeAssetList::ThemeAssetListIndex e)
				{
					switch (e)
					{
					case ThemeAssetList::BASIC_FONT: return "basic_font.ttf";
					case ThemeAssetList::MAIN_MENU_LOGO: return "main_menu_logo.png";
					case ThemeAssetList::CREATE_FILE: return "create_file.png";
					case ThemeAssetList::LOAD_FILE: return "load_file.png";
					case ThemeAssetList::SETTING: return "setting.png";
					case ThemeAssetList::CLOSE: return "close.png";
					default: return "unknown";
					}
				}
			};
		}
	}
}

#endif // VISUALNOVEL_ASSET_THEMEASSETLIST_H
