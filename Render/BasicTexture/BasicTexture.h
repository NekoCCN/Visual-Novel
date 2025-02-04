#pragma once
#ifndef VIUSLANOVEL_BASICTEXTURE_H
#define VIUSLANOVEL_BASICTEXTURE_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Core/Window/Window.h>
#include <Core/Exception/Exception.h>
#include <Asset/AssetPackRStream/AssetPackRStream.h>
#include <memory>
#include <vector>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
#include "../FontCore/FontCore.h"

namespace vn
{
	namespace render
	{
        class BasicTexture
        {
        private:
            SDL_Texture* texture_;

            BasicTexture(SDL_Texture* texture) : texture_(texture)
            {  }
            BasicTexture(const core::Window& window, const std::string& path)
            {
                texture_ = IMG_LoadTexture(window.getRendererHinding(), path.c_str());
            }
            BasicTexture(const core::Window& window, char* ptr, uint64_t size)
            {
                SDL_IOStream* rstream = SDL_IOFromMem(ptr, size);
                texture_ = IMG_LoadTexture_IO(window.getRendererHinding(), rstream, true);
            }
            BasicTexture(const core::Window& window, const asset::AssetPackRStream& asset_pack, uint64_t index)
            {
                std::vector<char> data;
                if (!asset_pack.getAssetBuffer(index, data))
                {
                    texture_ = nullptr;
                    throw core::exception::asset_pack_data_error();
                }

                SDL_IOStream* rstream = SDL_IOFromMem(data.data(), data.size());
                texture_ = IMG_LoadTexture_IO(window.getRendererHinding(), rstream, true);
            }
        public:
            BasicTexture(const BasicTexture&) = delete;
            BasicTexture& operator=(const BasicTexture&) = delete;
            BasicTexture(BasicTexture&& other) noexcept
            {
                texture_ = other.texture_;
                other.texture_ = nullptr;
            }
            BasicTexture& operator=(BasicTexture&& other) noexcept
            {
                texture_ = other.texture_;
                other.texture_ = nullptr;
                return *this;
            }
            ~BasicTexture()
            {
                if (texture_)
                {
                    SDL_DestroyTexture(texture_);
                }
            }
        };
	}
}

#endif // VIUSLANOVEL_BASICTEXTURE_H
