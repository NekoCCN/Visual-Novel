#include "EntryPoint.h"

bool vn::resolution::EntryPoint::getLine(std::string& buf) const
{
	buf.clear();
	unsigned char ch = '0';

	if (!SDL_ReadU8(rstream_, &ch))
	{
		return false;
	}

	if (ch != u8'\r')
	{
		buf.push_back(ch);
	}

	while (ch != u8'\n')
	{
		if (!SDL_ReadU8(rstream_, &ch))
		{
			return !buf.empty();
		}

		if (ch != u8'\r' && ch != u8'\n')
		{
			buf.push_back(ch);
		}
	}

	return true;
}

bool vn::resolution::EntryPoint::operator()(render::LogWindow& log_window)
{
	asset::AssetPackWStream wstream_(dst_path_, storage_);

	uint64_t line_number = 0;
	std::string line;

	std::string background_image;
	std::string background_sound;

	std::smatch res;

	defined_part_.resolve(rstream_, line_number, log_window);

	while (getLine(line))
	{
		++line_number;
        if (line.empty())
        {
            continue;
        }

		if (background_checker_.checkString(line))
		{
			if (std::regex_search(line, res, background_regex_))
			{
				std::string buf = res[1];
				if (!buf.empty())
				{
					background_image = buf;
				}
				buf = res[2];
                if (!buf.empty())
				{
					background_sound = buf;
				}

				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[Line: %ld] Background image: %s, Background sound: %s", line_number, background_image.c_str(), background_sound.c_str());
				log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] Background image: %s, Background sound: %s", line_number, background_image.c_str(), background_sound.c_str());
			}
		}
		else
		{
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[Line: %ld] Normal line: %s", line_number, line.c_str());
            log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] Normal line: %s", line_number, line.c_str());

			if (!normal_line_resolution_.resolve(line))
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[Line: %ld] Invalid normal line format", line_number);
				log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid normal line format", line_number);
				return false;
			}

			/*
				std::vector<std::string> character_name_list;

				std::vector<uint32_t> animate_speed;
				std::vector<std::vector<std::string>> character_asset_path_list;

				std::string background_image_path;
				std::string background_sound_path;

				std::string str;

				std::vector<core::command::CommandListEnum> command;
				std::vector<std::string> command_arguments;
				std::vector<std::string> command_asset_path;
			*/
			asset::AssetPackWStream::ProgramIndex buf;

			buf.character_name_list = normal_line_resolution_.getCharacterNameBuf();

			for (auto& x : normal_line_resolution_.getCharacterInnerNameBuf())
			{
				DefinedPart::AnimationPair pair_buf;
				const std::map<std::string, std::string>& image_map = defined_part_.getCharacterImageMap();
				const std::map<std::string, DefinedPart::AnimationPair>& animation_map = defined_part_.getCharacterAnimationMap();

				if (image_map.contains(x))
				{
					buf.character_asset_path_list.push_back({ image_map.at(x) });
					buf.animate_speed.push_back(0);
				}
				else if (animation_map.contains(x))
				{
					pair_buf = animation_map.at(x);
					buf.character_asset_path_list.push_back(pair_buf.first);
					buf.animate_speed.push_back(pair_buf.second);
				}
				else
				{
					SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "[Line: %ld] Character name not found", line_number);
                    log_window.log(SDL_LOG_PRIORITY_WARN, "[Line: %ld] Character name not found", line_number);
				}
			}

			buf.background_image_path = background_image;
            buf.background_sound_path = background_sound;

			buf.str = normal_line_resolution_.getStringBuf();

			for (auto& x : normal_line_resolution_.getCommandBuf())
			{
				core::command::CommandListEnum command_enum;
				if (!command_list_.resolve(x, command_enum))
				{
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[Line: %ld] Invalid command, Ignore.", line_number);
                    log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid command, Ignore.", line_number);
					continue;
				}
                buf.command.push_back(command_enum);
			}

			buf.command_arguments = normal_line_resolution_.getCommandArgumentsBuf();

            buf.command_asset_path = normal_line_resolution_.getCommandAssetPathBuf();

			try {
				wstream_ << buf;
			}
			catch (const asset::AssetPackWStream::chunk_error& e)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Line: %ld] Fatal error, failed to write resource.", line_number);
                log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Fatal error, failed to write resource.", line_number);
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Line: %ld] It maybe %s not found or disk error.", line_number, e.what());
                log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] It maybe %s not found or disk error.", line_number, e.what());
                return false;
			}
		}
	}

	wstream_.endFile();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[Line: %ld] End of file.", line_number);
    log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] End of file.", line_number);

	return true;
}
