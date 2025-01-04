#pragma once
#ifndef VISUALNOVEL_ASSET_THEMEPACK_H
#define VISUALNOVEL_ASSET_THEMEPACK_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

namespace vn
{
	namespace asset
	{
		namespace theme
		{
			class ThemePack
			{
			private:
				std::vector<std::shared_ptr<char>> asset_buffers_;
				std::vector<uint64_t> toc_;
				uint64_t toc_size_;

				SDL_IOStream* rstream_;

				std::string label_ = "This Is A Visual Novel Theme Pack File(.vntp)";
			public:
				ThemePack(const std::string& path);
				ThemePack() = delete;
				ThemePack(const ThemePack& other) = delete;
				ThemePack(ThemePack&& other) = delete;
				ThemePack& operator=(const ThemePack& other) = delete;
				ThemePack& operator=(ThemePack&& other) = delete;

				uint64_t getTOCSize() const
				{
					return toc_size_;
				}
				uint64_t getFileSize(uint64_t index) const
				{
					/*
					 * 1 2 3 4 5 6
					 * 0 1 3 6 10 15 21
					 */
					if (index >= toc_size_ || index < 0)
					{
						throw core::exception::asset_index_error();
					}

					return toc_[index + 1] - toc_[index];
				}
				// Can use the enum class ThemeAssetList
				std::shared_ptr<char> operator[](uint64_t index)
				{
					if (index >= toc_size_ || index < 0)
					{
						throw core::exception::asset_index_error();
					}

					return asset_buffers_[index];
				}
				~ThemePack()
				{
					SDL_CloseIO(rstream_);
				}
			};
		}
	}
}

#endif