#pragma execution_character_set("utf-8")
#include "ThemePack.h"

// Notice file can not read exception
vn::asset::theme::ThemePack::ThemePack(const std::string& path)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to open theme file : %s", path.c_str());
	rstream_ = SDL_IOFromFile(path.c_str(), "rb");
    if (rstream_ == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can not open file : %s", path.c_str());
        throw core::exception::can_not_read_file();
    }

    // Check File Format
    char* label_buf = new char[label_.size() + 1];
    SDL_ReadIO(rstream_, label_buf, label_.size() + 1);
    if (label_ != label_buf)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unknown file format");
        throw core::exception::invalid_file_format();
    }
    delete[] label_buf;

    // Read TOC size
    SDL_ReadIO(rstream_, &toc_size_, sizeof(uint64_t));

    // Read TOC
    for (uint64_t i = 0, buffer = 0; i < toc_size_; ++i)
    {
        if (SDL_ReadIO(rstream_, &buffer, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The file is damaged");
            throw core::exception::invalid_file_format();
        }
        toc_.push_back(buffer);
    }

    // Read file to buffer
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialize theme file : %s", path.c_str());
    for (uint64_t i = 0; i < toc_size_ - 1; ++i)
    {
        std::shared_ptr<char> buffer(new char[getFileSize(i)], std::default_delete<char[]>());
        if (SDL_ReadIO(rstream_, buffer.get(), getFileSize(i)) != getFileSize(i))
        {
            throw core::exception::invalid_file_format();
        }
        asset_buffers_.push_back(buffer);
    }
}