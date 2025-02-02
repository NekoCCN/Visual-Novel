#include "AssetPackRStream.h"

bool vn::asset::AssetPackRStream::readHeader()
{
    std::string label_buf(label_.size() + 1, '\0');
    if (SDL_ReadIO(rstream_, const_cast<char*>(label_buf.data()), label_.size() + 1) != label_.size() + 1)
    {
        return false;
    }
    if (std::string(label_buf.data()) != label_)
    {
        return false;
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "AssetPack(S): Version %.3lf", version_);
    
    double version;
    if (SDL_ReadIO(rstream_, &version, sizeof(double)) != sizeof(double))
    {
        return false;
    }

    if (label_buf.data() != label_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "AssetPack(S): Not a vnap format file", version_);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VERSION ERROR",
            "Vnap file is inconsistent with the program version", nullptr);
        return false;
    }
    if (version != version_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "AssetPack(S): vnap file is inconsistent with the program version", version_);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VERSION ERROR", 
            "Vnap file is inconsistent with the program version", nullptr);
        return false;
    }

    if (SDL_ReadIO(rstream_, &resources_offset_, sizeof(uint64_t)) != sizeof(uint64_t) ||
        SDL_ReadIO(rstream_, &toc_size_, sizeof(uint64_t)) != sizeof(uint64_t) ||
        SDL_ReadIO(rstream_, &character_name_size_, sizeof(uint64_t)) != sizeof(uint64_t) ||
        SDL_ReadIO(rstream_, &program_index_size_, sizeof(uint64_t)) != sizeof(uint64_t) ||
        SDL_ReadIO(rstream_, &extra_buffer_byte_size_, sizeof(uint64_t)) != sizeof(uint64_t))
    {
        return false;
    }

    return true;
}

bool vn::asset::AssetPackRStream::readTOC()
{
    if (SDL_SeekIO(rstream_, static_cast<int64_t>(resources_offset_), SDL_IO_SEEK_SET) < 0)
    {
        return false;
    }

    toc_.resize(toc_size_);
    for (uint64_t i = 0; i < toc_size_; ++i)
    {
        if (!SDL_ReadIO(rstream_, &toc_[i], sizeof(uint64_t)))
        {
            return false;
        }
    }

    return true;
}

bool vn::asset::AssetPackRStream::readCharacterNames()
{
    character_name_list_.reserve(character_name_size_);

    for (uint64_t i = 0; i < character_name_size_; ++i)
    {
        uint64_t str_size;
        if (SDL_ReadIO(rstream_, &str_size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        std::vector<char> str_buf(str_size);
        if (SDL_ReadIO(rstream_, str_buf.data(), str_size) != str_size)
        {
            return false;
        }

        character_name_list_.push_back(std::string(str_buf.data()));
    }

    return true;
}

bool vn::asset::AssetPackRStream::readProgramIndex()
{
    program_index_inner_list_.reserve(program_index_size_);

    for (uint64_t i = 0; i < program_index_size_; ++i)
    {
        ProgramIndexInner program_index;

        uint64_t size;
        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.character_name_index_list.resize(size);
        if (SDL_ReadIO(rstream_, program_index.character_name_index_list.data(),
            sizeof(uint64_t) * size) != sizeof(uint64_t) * size)
        {
            return false;
        }

        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.animate_speed.resize(size);
        if (SDL_ReadIO(rstream_, program_index.animate_speed.data(),
            sizeof(uint32_t) * size) != sizeof(uint32_t) * size)
        {
            return false;
        }

        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.character_asset_index_list.resize(size);
        for (auto& asset_list : program_index.character_asset_index_list)
        {
            uint64_t asset_size;
            if (SDL_ReadIO(rstream_, &asset_size, sizeof(uint64_t)) != sizeof(uint64_t))
            {
                return false;
            }

            asset_list.resize(asset_size);
            if (SDL_ReadIO(rstream_, asset_list.data(),
                sizeof(uint64_t) * asset_size) != sizeof(uint64_t) * asset_size)
            {
                return false;
            }
        }

        if (SDL_ReadIO(rstream_, &program_index.background_image_index, sizeof(uint64_t)) != sizeof(uint64_t) ||
            SDL_ReadIO(rstream_, &program_index.background_sound_index, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        if (SDL_ReadIO(rstream_, &program_index.string_index, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.command.resize(size);
        if (SDL_ReadIO(rstream_, program_index.command.data(),
            sizeof(core::command::CommandListEnum) * size) != sizeof(core::command::CommandListEnum) * size)
        {
            return false;
        }

        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.command_arguments.resize(size);
        for (auto& arg : program_index.command_arguments)
        {
            uint64_t arg_size;
            if (SDL_ReadIO(rstream_, &arg_size, sizeof(uint64_t)) != sizeof(uint64_t))
            {
                return false;
            }

            std::vector<char> arg_buf(arg_size);
            if (SDL_ReadIO(rstream_, arg_buf.data(), arg_size) != arg_size)
            {
                return false;
            }

            arg = std::string(arg_buf.data());
        }

        if (SDL_ReadIO(rstream_, &size, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            return false;
        }

        program_index.command_asset_index.resize(size);
        if (SDL_ReadIO(rstream_, program_index.command_asset_index.data(),
            sizeof(uint64_t) * size) != sizeof(uint64_t) * size)
        {
            return false;
        }

        program_index_inner_list_.push_back(program_index);
    }

    return true;
}

bool vn::asset::AssetPackRStream::readExtraArea(std::shared_ptr<const ExtraArea> extra_area) const
{
    if (extra_area != nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Extra area pointer already exist address.");
        return false;
    }
    if (extra_buffer_byte_size_ == 0)
    {
        return true;
    }

    std::vector<char> buffer(extra_buffer_byte_size_);

    if (SDL_ReadIO(rstream_, buffer.data(), extra_buffer_byte_size_) != extra_buffer_byte_size_)
    {
        return false;
    }

    try
    {
        extra_area = ExtraArea::loadFromData(buffer);
    }
    catch (const std::exception& e)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load extra area: %s", e.what());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "EXTRA AREA ERROR",
            "Failed to load extra area: %s", nullptr);
        return false;
    }

    return true;
}


std::string vn::asset::AssetPackRStream::readString(uint64_t index) const
{
    if (index >= toc_.size())
    {
        throw chunk_error("Invalid string index");
    }

    uint64_t size = toc_[index + 1] - toc_[index];
    std::string buffer(size, '\0');

    if (!readChunk(index, const_cast<char*>(buffer.data()), size))
    {
        throw chunk_error("Failed to read string");
    }

    return buffer;
}

bool vn::asset::AssetPackRStream::readChunk(uint64_t index, char* buffer, uint64_t size) const
{
    if (index >= toc_.size())
    {
        return false;
    }

    if (SDL_SeekIO(rstream_, toc_[index], SDL_IO_SEEK_SET) < 0)
    {
        return false;
    }

    if (SDL_ReadIO(rstream_, buffer, size) != size)
    {
        return false;
    }

    return true;
}

bool vn::asset::AssetPackRStream::getAsset(uint64_t index, std::vector<char>& buffer) const
{
    if (index >= toc_.size())
    {
        return false;
    }

    uint64_t size = toc_[index + 1] - toc_[index];
    buffer.resize(size);

    return readChunk(index, buffer.data(), size);
}

std::string vn::asset::AssetPackRStream::getCharacterName(uint64_t index) const
{
    if (index >= character_name_list_.size())
    {
        throw chunk_error("Invalid character name index");
    }
    return character_name_list_[index];
}

const vn::asset::AssetPackRStream::ProgramIndexInner& vn::asset::AssetPackRStream::getProgramIndex(uint64_t index) const
{
    if (index >= program_index_inner_list_.size())
    {
        throw chunk_error("Invalid program index");
    }
    return program_index_inner_list_[index];
}