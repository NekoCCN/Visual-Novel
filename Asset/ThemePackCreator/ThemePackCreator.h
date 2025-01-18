#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_THEMEPACKCREATOR_H
#define VISUALNOVEL_THEMEPACKCREATOR_H
#include <Core/Exception/Exception.h>
#include "../ThemeAssetList/ThemeAssetList.h"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <string>

namespace vn
{
	namespace asset
	{
		namespace theme
		{
			class ThemePackCreatorInterface
			{
			protected:
				// File IO
				SDL_IOStream* wstream_ = nullptr;
				SDL_Storage* storage_ = nullptr;

				// TOC
				std::vector<uint64_t> toc_;
				uint64_t toc_size_ = 0;

				// Label
				std::string label_ = "This Is A Visual Novel Theme Pack File(.vntp)";
			public:
				// Constructor Declare
				ThemePackCreatorInterface() = default;
				ThemePackCreatorInterface(const ThemePackCreatorInterface& other) = delete;
				ThemePackCreatorInterface(ThemePackCreatorInterface&& other) = delete;
				ThemePackCreatorInterface& operator=(const ThemePackCreatorInterface& other) = delete;
				ThemePackCreatorInterface& operator=(ThemePackCreatorInterface&& other) = delete;

				// Founction
				virtual std::string platformName() = 0;
				bool operator()();
				virtual ~ThemePackCreatorInterface() = default;
			};
			class ThemePackCreator : public ThemePackCreatorInterface
			{
			public:
				// Constructor Declare
				ThemePackCreator() = delete;
				ThemePackCreator(const ThemePackCreator& other) = delete;
				ThemePackCreator(ThemePackCreator&& other) = delete;
				ThemePackCreator& operator=(const ThemePackCreator& other) = delete;
				ThemePackCreator& operator=(ThemePackCreator&& other) = delete;

				// Founction
				std::string platformName() override
				{
					return "Not android";
				}
				ThemePackCreator(const std::string& file_path, const std::string& storage_path);
				~ThemePackCreator() override
				{
					SDL_CloseIO(wstream_);
					SDL_CloseStorage(storage_);
				}
			};
		}
	}
}
#endif // VISUALNOVEL_THEMEPACKCREATOR_H
