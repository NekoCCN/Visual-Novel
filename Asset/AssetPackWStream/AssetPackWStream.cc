#include "AssetPackWStream.h"

bool vn::asset::AssetPackWStream::chunkWriteByPath(const std::string& path, uint64_t& return_index)
{
    if (path_index_table_.find(path) != path_index_table_.end())
    {
        return_index = path_index_table_.at(path);
        return true;
    }

    SDL_IOStream* rstream = SDL_IOFromFile(path.c_str(), "rb");
    if (rstream == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open file %s", path.c_str());
        return false;
    }

    uint64_t size = SDL_GetIOSize(rstream);
    if (size <= 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get file size %s", path.c_str());
        SDL_CloseIO(rstream);
        return false;
    }

    char* buffer = nullptr;

    try
    {
        // First try with full size
        buffer = new char[size];

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using full buffer size for file %s", path.c_str());

        if (SDL_ReadIO(rstream, buffer, size) != size)
        {
            if (buffer)
            {
                delete[] buffer;
                buffer = nullptr;
            }
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file %s", path.c_str());
            SDL_CloseIO(rstream);
            return false;
        }
        
        SDL_WriteIO(wstream_, buffer, size);
    }
    catch (const std::bad_alloc&)
    {
        // If full buffer allocation fails, try chunked approach
        if (buffer)
        {
            delete[] buffer;
            buffer = nullptr;
        }

        try
        {
            // Try with MAX_BUFFER_SIZE
            buffer = new char[MAX_BUFFER_SIZE];

            uint64_t remaining_size = size;

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using normal chunk write for file %s", path.c_str());
            
            while (remaining_size > 0)
            {
                uint32_t chunk_size = std::min(remaining_size, MAX_BUFFER_SIZE);
                if (SDL_ReadIO(rstream, buffer, chunk_size) != chunk_size)
                {
                    if (buffer)
                    {
                        delete[] buffer;
                        buffer = nullptr;
                    }
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file %s", path.c_str());
                    SDL_CloseIO(rstream);
                    return false;
                }
                SDL_WriteIO(wstream_, buffer, chunk_size);
                remaining_size -= chunk_size;
            }

        }
        catch (const std::bad_alloc&)
        {
            // If MAX_BUFFER_SIZE fails, try with minimum buffer
            if (buffer)
            {
                delete[] buffer;
                buffer = nullptr;
            }

            try {
                buffer = new char[ERROR_BUFFER_SIZE];

                uint64_t remaining_size = size;
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using error chunk buffer for file %s", path.c_str());

                while (remaining_size > 0)
                {
                    uint32_t chunk_size = std::min(remaining_size, ERROR_BUFFER_SIZE);
                    if (SDL_ReadIO(rstream, buffer, chunk_size) != chunk_size)
                    {
                        if (buffer)
                        {
                            delete[] buffer;
                            buffer = nullptr;
                        }
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file %s", path.c_str());
                        SDL_CloseIO(rstream);
                        return false;
                    }
                    SDL_WriteIO(wstream_, buffer, chunk_size);
                    remaining_size -= chunk_size;
                }

            }
            catch (const std::bad_alloc&)
            {
                if (buffer)
                {
                    delete[] buffer;
                    buffer = nullptr;
                }
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate buffer for file %s", path.c_str());
                SDL_CloseIO(rstream);
                return false;
            }
        }
    }

    toc_.push_back(toc_[toc_.size() - 1] + size);
    return_index = toc_.size() - 1;
    path_index_table_.insert(std::make_pair(path, toc_.size() - 1));

    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }

    SDL_CloseIO(rstream);

    return true;
}
bool vn::asset::AssetPackWStream::chunkWriteByStorage(const std::string& path, uint64_t& return_index)
{
    if (path_index_table_.find(path) != path_index_table_.end())
    {
        return_index = path_index_table_.at(path);
        return true;
    }

    uint64_t size = 0;
    if (!SDL_GetStorageFileSize(storage_, path.c_str(), &size))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get storage file size %s", path.c_str());
        return false;
    }

    char* buffer = nullptr;

    try
    {
        // First try with full size
        buffer = new char[size];

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using full buffer size for storage file %s", path.c_str());

        if (!SDL_ReadStorageFile(storage_, path.c_str(), buffer, size))
        {
            if (buffer)
            {
                delete[] buffer;
                buffer = nullptr;
            }
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read storage file %s", path.c_str());
            return false;
        }

        SDL_WriteIO(wstream_, buffer, size);
    }
    catch (const std::bad_alloc&)
    {
        // If full buffer allocation fails, try chunked approach
        if (buffer)
        {
            delete[] buffer;
            buffer = nullptr;
        }

        try
        {
            // Try with MAX_BUFFER_SIZE
            buffer = new char[MAX_BUFFER_SIZE];

            uint64_t remaining_size = size;

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using normal chunk write for storage file %s", path.c_str());

            while (remaining_size > 0)
            {
                uint32_t chunk_size = std::min(remaining_size, MAX_BUFFER_SIZE);
                if (!SDL_ReadStorageFile(storage_, path.c_str(), buffer, chunk_size))
                {
                    if (buffer)
                    {
                        delete[] buffer;
                        buffer = nullptr;
                    }
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read storage file %s", path.c_str());
                    return false;
                }
                SDL_WriteIO(wstream_, buffer, chunk_size);
                remaining_size -= chunk_size;
            }

        }
        catch (const std::bad_alloc&)
        {
            // If MAX_BUFFER_SIZE fails, try with minimum buffer
            if (buffer)
            {
                delete[] buffer;
                buffer = nullptr;
            }

            try {
                buffer = new char[ERROR_BUFFER_SIZE];

                uint64_t remaining_size = size;
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using error chunk buffer for storage file %s", path.c_str());

                while (remaining_size > 0)
                {
                    uint32_t chunk_size = std::min(remaining_size, ERROR_BUFFER_SIZE);
                    if (!SDL_ReadStorageFile(storage_, path.c_str(), buffer, chunk_size))
                    {
                        if (buffer)
                        {
                            delete[] buffer;
                            buffer = nullptr;
                        }
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read storage file %s", path.c_str());
                        return false;
                    }
                    SDL_WriteIO(wstream_, buffer, chunk_size);
                    remaining_size -= chunk_size;
                }

            }
            catch (const std::bad_alloc&)
            {
                if (buffer)
                {
                    delete[] buffer;
                    buffer = nullptr;
                }
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate buffer for storage file %s", path.c_str());
                return false;
            }
        }
    }

    toc_.push_back(toc_[toc_.size() - 1] + size);
    return_index = toc_.size() - 1;

    path_index_table_.insert(std::make_pair(path, toc_.size() - 1));

    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }

    return true;
}