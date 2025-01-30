#include "AssetPackRStream.h"

bool vn::asset::AssetPackRStream::readProgramIndex()
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

				if (!ReadIO(const_cast<char*>(arg.data()), buf))
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
