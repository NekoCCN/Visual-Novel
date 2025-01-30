#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_ASSETPACKRSTREAM_H
#define VISUALNOVEL_ASSETPACKRSTREAM_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <Core/CommandList/CommandList.h>
#include <vector>
#include <utility>
#include <string>
#include <exception>

namespace vn
{
    namespace asset
    {
        class AssetPackRStream
        {
        public:
            struct ProgramIndex
            {
                std::vector<std::string> character_name_list;

                std::vector<uint32_t> animate_speed;
                std::vector<std::vector<std::string>> character_asset_path_list;

                std::string background_image_path;
                std::string background_sound_path;

                std::string str;

                std::vector<core::command::CommandListEnum> command;
                std::vector<std::string> command_arguments;
                std::vector<std::string> command_asset_path;
            };

            class chunk_error : std::exception
            {
            private:
                std::string message;
            public:
                chunk_error(const std::string& message = "NULL") : message(message)
                {  }
                const char* what() const noexcept
                {
                    return message.c_str();
                }
            };

        private:
            SDL_IOStream* rstream_;
            std::vector<uint64_t> toc_;
            std::vector<std::string> character_name_list_;
            std::vector<ProgramIndex> program_index_list_;
            
            std::string label_ = "VN_PACK";
            uint64_t resources_offset_;
            uint64_t toc_size_;
            uint64_t character_name_size_;
            uint64_t program_index_size_;

            static constexpr uint64_t MAX_BUFFER_SIZE = 1024ll * 1024ll * 10ll;     // 10MB
            static constexpr uint64_t ERROR_BUFFER_SIZE = 1024ll * 1024ll;          // 1MB fallback buffer

            bool readHeader()
            {
                std::string label_buf;
                label_buf.resize(label_.size() + 1);
                
                if (SDL_ReadIO(rstream_, const_cast<char*>(label_buf.data()), label_.size() + 1) != label_.size() + 1)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to read label");
                    return false;
                }

                if (label_buf != label_)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Invalid label");
                    return false;
                }

                if (SDL_ReadIO(rstream_, &resources_offset_, sizeof(uint64_t)) != sizeof(uint64_t) ||
                    SDL_ReadIO(rstream_, &toc_size_, sizeof(uint64_t)) != sizeof(uint64_t) ||
                    SDL_ReadIO(rstream_, &character_name_size_, sizeof(uint64_t)) != sizeof(uint64_t) ||
                    SDL_ReadIO(rstream_, &program_index_size_, sizeof(uint64_t)) != sizeof(uint64_t))
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to read header");
                    return false;
                }

                return true;
            }

            bool readToc()
            {
                toc_.resize(toc_size_);
                if (SDL_ReadIO(rstream_, toc_.data(), sizeof(uint64_t) * toc_size_) != sizeof(uint64_t) * toc_size_)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to read TOC");
                    return false;
                }
                return true;
            }

            bool readCharacterName()
            {

            }

            bool readProgramIndex();

        public:
            AssetPackRStream(const std::string& src_path)
            {
                rstream_ = SDL_IOFromFile(src_path.c_str(), "rb");
                if (rstream_ == nullptr)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open file: %s", src_path.c_str());
                    throw core::exception::file_not_found_error();
                }

                if (!readHeader() || !readToc() || !readProgramIndex())
                {
                    SDL_CloseIO(rstream_);
                    throw chunk_error("Failed to read asset pack");
                }
            }

            ~AssetPackRStream()
            {
                if (rstream_)
                {
                    SDL_CloseIO(rstream_);
                }
            }

            // Access methods
            size_t getProgramIndexCount() const
            {
                return program_index_list_.size();
            }
            
            const ProgramIndex& getProgramIndex(size_t index) const
            {
                if (index >= program_index_list_.size())
                {
                    throw chunk_error("Invalid program index");
                }
                return program_index_list_[index];
            }

            std::string readString(uint64_t index) const
            {
                if (index >= toc_.size() - 1)
                {
                    throw chunk_error("Invalid string index");
                }

                uint64_t size = toc_[index + 1] - toc_[index];
                std::string str;
                str.resize(size);

                SDL_SeekIO(rstream_, resources_offset_ + toc_[index], SDL_IO_SEEK_SET);
                if (SDL_ReadIO(rstream_, const_cast<char*>(str.data()), size) != size)
                {
                    throw chunk_error("Failed to read string");
                }

                return str;
            }

            std::vector<char>&& readChunk(uint64_t index) const
            {
                if (index >= toc_.size() - 1)
                {
                    throw chunk_error("Invalid chunk index");
                }

                uint64_t size = toc_[index + 1] - toc_[index];
                std::vector<char> data(size);

                SDL_SeekIO(rstream_, resources_offset_ + toc_[index], SDL_IO_SEEK_SET);
                if (SDL_ReadIO(rstream_, data.data(), size) != size)
                {
                    throw chunk_error("Failed to read chunk");
                }

                return std::move(data);
            }

            std::string getCharacterName(uint64_t index) const
            {
                if (index >= character_name_list_.size())
                {
                    throw chunk_error("Invalid character name index");
                }

                return character_name_list_[index];
            }
        };
    }
}

#endif // VISUALNOVEL_ASSETPACKRSTREAM_H