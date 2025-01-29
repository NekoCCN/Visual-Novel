#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_ASSETPACKRSTREAM_H
#define VISUALNOVEL_ASSETPACKRSTREAM_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <Core/CommandList/CommandList.h>
#include <vector>
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
            uint64_t program_index_size_;

            static constexpr uint64_t MAX_BUFFER_SIZE = 1024ll * 1024ll * 10ll;     // 10MB
            static constexpr uint64_t ERROR_BUFFER_SIZE = 1024ll * 1024ll;          // 1MB fallback buffer

            bool readHeader()
            {
                std::string label_buf;
                label_buf.resize(label_.size() + 1);
                
                if (SDL_ReadIO(rstream_, label_buf.data(), label_.size() + 1) != label_.size() + 1)
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

            bool readProgramIndex()
            {
                auto ReadIO = [&](void* data, size_t size) -> bool
                {
                    return SDL_ReadIO(rstream_, data, size) == size;
                };

                try
                {
                    for (uint64_t i = 0; i < program_index_size_; i++)
                    {
                        ProgramIndex program_index;
                        uint64_t buf = 0;

                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        std::vector<uint64_t> character_name_indices(buf);
                        for (auto& idx : character_name_indices)
                        {
                            if (!ReadIO(&idx, sizeof(uint64_t)))
                            {
                                return false;
                            }
                            if (idx >= character_name_list_.size())
                            {
                                return false;
                            }
                            program_index.character_name_list.push_back(character_name_list_[idx]);
                        }

                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        program_index.animate_speed.resize(buf);
                        for (auto& speed : program_index.animate_speed)
                        {
                            if (!ReadIO(&speed, sizeof(uint32_t)))
                            {
                                return false;
                            }
                        }

                        // Read character asset indices
                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        uint64_t asset_list_size = buf;
                        program_index.character_asset_path_list.resize(asset_list_size);
                        
                        for (uint64_t j = 0; j < asset_list_size; j++)
                        {
                            if (!ReadIO(&buf, sizeof(uint64_t)))
                            {
                                return false;
                            }
                            std::vector<uint64_t> asset_indices(buf);
                            for (auto& idx : asset_indices)
                            {
                                if (!ReadIO(&idx, sizeof(uint64_t)))
                                {
                                    return false;
                                }
                            }
                        }

                        // Read background indices
                        uint64_t bg_image_idx, bg_sound_idx;
                        if (!ReadIO(&bg_image_idx, sizeof(uint64_t)) ||
                            !ReadIO(&bg_sound_idx, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        // Convert indices to content when accessed

                        // Read string index
                        uint64_t string_idx;
                        if (!ReadIO(&string_idx, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        program_index.str = readString(string_idx);

                        // Read commands
                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        program_index.command.resize(buf);
                        for (auto& cmd : program_index.command)
                        {
                            if (!ReadIO(&cmd, sizeof(core::command::CommandListEnum)))
                            {
                                return false;
                            }
                        }

                        // Read command arguments
                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }
                        uint64_t arg_count = buf;
                        program_index.command_arguments.resize(arg_count);
                        
                        for (uint64_t j = 0; j < arg_count; j++)
                        {
                            if (!ReadIO(&buf, sizeof(uint64_t)))
                            {
                                return false;
                            }

                            std::string arg;
                            arg.resize(buf);

                            if (!ReadIO(arg.data(), buf))
                            {
                                return false;
                            }

                            program_index.command_arguments[j] = arg;
                        }

                        // Read command asset indices
                        if (!ReadIO(&buf, sizeof(uint64_t)))
                        {
                            return false;
                        }

                        std::vector<uint64_t> cmd_asset_indices(buf);
                        for (auto& idx : cmd_asset_indices)
                        {
                            if (!ReadIO(&idx, sizeof(uint64_t)))
                            {
                                return false;
                            }
                        }

                        program_index_list_.push_back(program_index);
                    }
                }
                catch (const std::exception& e)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to read program indices: %s", e.what());
                    return false;
                }

                return true;
            }

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
                if (SDL_ReadIO(rstream_, str.data(), size) != size)
                {
                    throw chunk_error("Failed to read string");
                }

                return str;
            }

            std::vector<char> readChunk(uint64_t index) const
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

                return data;
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