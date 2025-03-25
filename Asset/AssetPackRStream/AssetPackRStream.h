#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_ASSETPACKRSTREAM_H
#define VISUALNOVEL_ASSETPACKRSTREAM_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <Core/CommandList/CommandList.h>
#include "../ExtraArea/ExtraArea.h"
#include <vector>
#include <string>

namespace vn
{
    namespace asset
    {
        class AssetPackRStream
        {
        public:
            struct ProgramConfigInner
            {
                char name[50];

                int32_t width;
                int32_t height;

                int32_t fps;
                int32_t is_vsync;

                uint64_t font_index;
                uint64_t dialog_box_index;
            };

            struct ProgramIndexInner
            {
                std::vector<uint64_t> character_name_index_list;

                std::vector<uint32_t> animate_speed;
                std::vector<std::vector<uint64_t>> character_asset_index_list;

                int64_t background_image_index;
                int64_t background_sound_index;

                int64_t string_index;

                std::vector<core::command::CommandListEnum> command;
                std::vector<std::string> command_arguments;
                std::vector<uint64_t> command_asset_index;
            };

            class chunk_error
            {
            private:
                std::string message;
            public:
                chunk_error(const std::string& message = "NULL") : message(message) {}
                const char* what() const noexcept
                {
                    return message.c_str();
                }
            };

        private:
            SDL_IOStream* rstream_;

            inline static constexpr double version_ = 1.2;
            inline static const std::string label_ = "VN_PACK";

            uint64_t resources_offset_;
            uint64_t toc_size_;
            uint64_t character_name_size_;
            uint64_t program_index_size_;
            uint64_t extra_buffer_byte_size_;

            std::vector<uint64_t> toc_;
            std::vector<std::string> character_name_list_;
            std::vector<ProgramIndexInner> program_index_inner_list_;

            static constexpr uint64_t MAX_BUFFER_SIZE = 1024ll * 1024ll * 10ll;     // 10MB
            static constexpr uint64_t ERROR_BUFFER_SIZE = 1024ll * 1024ll;          // 1MB

            bool readHeader();
            bool readTOC();
            bool readCharacterNames();
            bool readProgramIndex();
            bool readExtraArea(std::shared_ptr<const ExtraArea> extra_area = nullptr) const;

            std::string readString(uint64_t index) const;
            bool readChunk(uint64_t index, char* buffer, uint64_t size) const;

        public:
            AssetPackRStream(const std::string& src_path)
            {
                rstream_ = SDL_IOFromFile(src_path.c_str(), "rb");
                if (rstream_ == nullptr)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackRStream: Failed to open file %s", src_path.c_str());
                    throw core::exception::file_not_found_error();
                }

                if (!readHeader() || !readTOC() || !readCharacterNames() || !readProgramIndex())
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackRStream: Failed to read file %s", src_path.c_str());
                    throw chunk_error("Failed to read file");
                }
            }

            ~AssetPackRStream()
            {
                SDL_CloseIO(rstream_);
            }

            bool getAssetBuffer(uint64_t index, std::vector<char>& buffer) const;
            bool getAssetPointer(uint64_t index, std::shared_ptr<char>& pointer) const;

            uint64_t getAssetSize(uint64_t index) const
            {
                return toc_[index + 1] - toc_[index];
            }

            std::string getCharacterName(uint64_t index) const;

            const std::vector<std::string>& getAllCharacterNames() const
            {
                return character_name_list_;
            }

            const ProgramIndexInner& getProgramIndex(uint64_t index) const;

            uint64_t getProgramIndexCount() const
            {
                return program_index_size_;
            }

            std::string getString(uint64_t index) const
            {
                return readString(index);
            }

            static double getVersion()
            {
                return version_;
            }
        };
    }
}

#endif VISUALNOVEL_ASSETPACKRSTREAM_H