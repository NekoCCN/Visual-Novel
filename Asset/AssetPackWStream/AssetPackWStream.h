#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_ASSETPACKWSTREAM_H
#define VISUALNOVEL_ASSETPACKWSTREAM_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <vector>
#include <string>
#include <map>

namespace vn
{
	namespace asset
	{
		class AssetPackWStream
        {
		public:
			struct ProgramIndex
			{
				std::vector<uint32_t> animate_speed;
				std::vector<std::string> character_name_list;
				std::vector<std::vector<std::string>> character_asset_path_list;

				std::string background_image_path;
				std::string background_sound_path;

				std::vector<std::string> other_sound;

				std::string command;
				std::vector<std::string> command_arguments;
			};
			struct ProgramIndexInner
			{
				std::vector<uint32_t> animate_speed;
				std::vector<uint64_t> character_name_index_list;
				std::vector<std::vector<uint64_t>> character_asset_index_list;

				uint64_t background_image_index;
				uint64_t background_sound_index;

				std::vector<uint64_t> other_sound_index;

				std::string command;
				std::vector<std::string> command_arguments;
			};
			class chunk_error
			{
				std::string message;
				chunk_error(const std::string& message = "NULL") : message(message) {}
				const char* what() const noexcept
				{
					return message.c_str();
				}
			};
		private:

			SDL_IOStream* wstream_;
			
			std::map<std::string, uint64_t> path_index_table_;

			std::vector<uint64_t> toc_;

			std::vector<std::string> character_name_list_;
			std::map<std::string, uint64_t> character_name_table_;

			std::vector<ProgramIndex> program_index_list_;

			static constexpr uint64_t MAX_BUFFER_SIZE = 1024 * 1024 * 10;     // 10MB
			static constexpr uint64_t ERROR_BUFFER_SIZE = 1024 * 1024;          // 1MB fallback buffer

			bool chunkWriteByPath(const std::string& path, uint64_t& return_index);
			bool chunkWriteByStorage(const std::string& path, uint64_t& return_index);

			bool haveStorage_;
			SDL_Storage* storage_;
		public:
			AssetPackWStream(const std::string& dst_path, SDL_Storage* storage = nullptr)
			{
				toc_.push_back(0);

				if (storage_ == nullptr)
				{
					haveStorage_ = false;

					wstream_ = SDL_IOFromFile(dst_path.c_str(), "rb");
					if (wstream_ != nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File name exist : %s", dst_path.c_str());
						SDL_CloseIO(wstream_);
						throw core::exception::file_existed_error();
					}
					SDL_CloseIO(wstream_);

					wstream_ = SDL_IOFromFile(dst_path.c_str(), "wb");
					if (wstream_ == nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackWStream: Failed to open file %s", dst_path.c_str());
						throw core::exception::file_not_found_error();
					}
				}
				else
				{
					haveStorage_ = true;

					if (!SDL_StorageReady(storage))
					{
						haveStorage_ = false;
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackWStream: Wrong storage, Ignore", dst_path.c_str());
					}

					storage_ = storage;
					wstream_ = SDL_IOFromFile(dst_path.c_str(), "rb");
					if (wstream_ != nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File name exist : %s", dst_path.c_str());
						SDL_CloseIO(wstream_);
						throw core::exception::file_existed_error();
					}
					SDL_CloseIO(wstream_);

					wstream_ = SDL_IOFromFile(dst_path.c_str(), "wb");
					if (wstream_ == nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackWStream: Failed to open file %s", dst_path.c_str());
						throw core::exception::file_not_found_error();
					}
				}
			}
			AssetPackWStream& operator<<(const ProgramIndex& program_index);
			~AssetPackWStream()
			{
				SDL_CloseIO(wstream_);
			}
        };
	}
}

#endif // VISUALNOVEL_ASSETPACKWSTREAM_H