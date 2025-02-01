#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_ASSETPACKWSTREAM_H
#define VISUALNOVEL_ASSETPACKWSTREAM_H
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <Core/CommandList/CommandList.h>
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

			SDL_IOStream* wstream_;
			
			std::map<std::string, uint64_t> path_index_table_;

			std::vector<uint64_t> toc_;

			std::vector<std::string> character_name_list_;
			std::map<std::string, uint64_t> character_name_table_;

			std::vector<ProgramIndexInner> program_index_inner_list_;

			std::string label_ = "VN_PACK";

			static constexpr uint64_t MAX_BUFFER_SIZE = 1024ll * 1024ll * 10ll;     // 10MB
			static constexpr uint64_t ERROR_BUFFER_SIZE = 1024ll * 1024ll;          // 1MB fallback buffer

			bool chunkWriteByPath(const std::string& path, uint64_t& return_index);
			bool chunkWriteByStorage(const std::string& path, uint64_t& return_index);
			uint64_t chunkWrite(const std::string& path);
			uint64_t WriteString(const std::string& str)
			{
				if (SDL_WriteIO(wstream_, str.c_str(), str.size() + 1) != str.size() + 1)
				{
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to write string");
                    throw chunk_error();
				}
				toc_.push_back(toc_[toc_.size() - 1] + str.size() + 1);
				return toc_.size() - 2;
			}

			bool haveStorage_;
			SDL_Storage* storage_;

			bool is_done_ = false;
		public:
			AssetPackWStream(const std::string& dst_path, SDL_Storage* storage = nullptr) : storage_(storage)	
			{

				if (storage_ == nullptr)
				{
					haveStorage_ = false;

					wstream_ = SDL_IOFromFile(dst_path.c_str(), "wb");
					if (wstream_ == nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackWStream: Failed to open file %s", dst_path.c_str());
						throw core::exception::file_not_found_error();
					}
					if (SDL_TellIO(wstream_) != 0)
					{
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File name exist : %s", dst_path.c_str());
						SDL_CloseIO(wstream_);
						throw core::exception::file_existed_error();
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

					wstream_ = SDL_IOFromFile(dst_path.c_str(), "wb");
					if (wstream_ == nullptr)
					{
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "AssetPackWStream: Failed to open file %s", dst_path.c_str());
						throw core::exception::file_not_found_error();
					}
					if (SDL_GetIOSize(wstream_) != 0)
					{
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File name exist : %s", dst_path.c_str());
						SDL_CloseIO(wstream_);
						throw core::exception::file_existed_error();
					}
				}

				constexpr uint64_t resources_offset = 0;
				constexpr uint64_t toc_size = 0;
				constexpr uint64_t character_name_size = 0;
				constexpr uint64_t program_index_size = 0;
				SDL_WriteIO(wstream_, label_.c_str(), label_.size() + 1);
				SDL_WriteIO(wstream_, &resources_offset, sizeof(uint64_t));
                SDL_WriteIO(wstream_, &toc_size, sizeof(uint64_t));
				SDL_WriteIO(wstream_, &character_name_size, sizeof(uint64_t));
				SDL_WriteIO(wstream_, &program_index_size, sizeof(uint64_t));

				toc_.push_back(SDL_TellIO(wstream_));
			}

			AssetPackWStream& operator<<(const ProgramIndex& program_index);
			/*
				std::vector<uint64_t> character_name_index_list;

				std::vector<uint32_t> animate_speed;
				std::vector<std::vector<uint64_t>> character_asset_index_list;

				uint64_t background_image_index;
				uint64_t background_sound_index;

				uint64_t string_index;

				std::vector<uint64_t> other_sound_index;

				std::vector<core::command::CommandListEnum> command;
				std::vector<std::string> command_arguments;
				std::vector<uint64_t> command_asset_index;
			*/
			void endFile()
			{
				if (is_done_ == true)
				{
					return;
				}

				auto WriteIO = [&](SDL_IOStream* stream, const void* data, size_t size) -> void
				{
					if (SDL_WriteIO(stream, data, size) != size)
					{
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to write data");
						throw chunk_error();
					}
				};

				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "AssetPackWStream: End file");

				SDL_SeekIO(wstream_, 0, SDL_IO_SEEK_END);
				uint64_t tmp = SDL_TellIO(wstream_);

				SDL_SeekIO(wstream_, 0, SDL_IO_SEEK_SET);

				WriteIO(wstream_, label_.data(), label_.size() + 1);
                SDL_WriteIO(wstream_, &tmp, sizeof(uint64_t));
				tmp = toc_.size();
                SDL_WriteIO(wstream_, &tmp, sizeof(uint64_t));
				tmp = character_name_list_.size();
                SDL_WriteIO(wstream_, &tmp, sizeof(uint64_t));
				tmp = program_index_inner_list_.size();
                SDL_WriteIO(wstream_, &tmp, sizeof(uint64_t));

				SDL_SeekIO(wstream_, 0, SDL_IO_SEEK_END);

				try
				{
					WriteIO(wstream_, toc_.data(), sizeof(uint64_t) * toc_.size());
					
					for (auto& x : character_name_list_)
					{
						uint64_t size = x.size() + 1;
						WriteIO(wstream_, &size, sizeof(uint64_t));
                        WriteIO(wstream_, x.c_str(), x.size() + 1);
					}

					uint64_t buf = 0;
					for (auto& x : program_index_inner_list_)
					{
						buf = x.character_name_index_list.size();
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						for (auto& y : x.character_name_index_list)
						{
							WriteIO(wstream_, &y, sizeof(uint64_t));
						}

						buf = x.animate_speed.size();
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						for (auto& y : x.animate_speed)
						{
							WriteIO(wstream_, &y, sizeof(uint32_t));
						}

						buf = x.character_asset_index_list.size();
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						for (auto& y : x.character_asset_index_list)
						{
							buf = y.size();
							WriteIO(wstream_, &buf, sizeof(uint64_t));
							for (auto& z : y)
							{
								WriteIO(wstream_, &z, sizeof(uint64_t));
							}
						}

						buf = x.background_image_index;
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						buf = x.background_sound_index;
						WriteIO(wstream_, &buf, sizeof(uint64_t));

						buf = x.string_index;
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						
						buf  = x.command.size();
                        WriteIO(wstream_, &buf, sizeof(uint64_t));
                        for (auto& y : x.command)
						{
                            WriteIO(wstream_, &y, sizeof(core::command::CommandListEnum));
                        }

						buf = x.command_arguments.size();
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						for (auto& y : x.command_arguments)
						{
							buf = y.size() + 1;
							WriteIO(wstream_, &buf, sizeof(uint64_t));
							WriteIO(wstream_, y.c_str(), buf);
						}

						buf = x.command_asset_index.size();
						WriteIO(wstream_, &buf, sizeof(uint64_t));
						for (auto& y : x.command_asset_index)
						{
							WriteIO(wstream_, &y, sizeof(uint64_t));
						}
					}
				}
				catch (const chunk_error& e)
				{
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EndFile error: %s", e.what());
					throw;
				}

				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "AssetPackWStream: End file done");

				is_done_ = true;
			}

			~AssetPackWStream()
			{
				endFile();
				SDL_CloseIO(wstream_);
				SDL_CloseStorage(storage_);
			}
        };
	}
}

#endif // VISUALNOVEL_ASSETPACKWSTREAM_H