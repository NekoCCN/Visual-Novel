#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_NORMALLINERESOLUTION_H
#define VISUALNOVEL_NORMALLINERESOLUTION_H
#include <SDL3/SDL.h>
#include <regex>
#include <string>
#include <vector>
#include <Render/LogWindow/LogWindow.h>
#include "../FormatChecker/FormatChecker.h"

namespace vn
{
	namespace resolution
	{
		class NormalLineResolution
		{
		private:
			RegexChecker normal_line_regex_;

			std::regex character_regex_;

			std::regex plural_regex_;

			std::regex command_plural_regex_;

			std::regex command_regex_;

			std::regex string_regex_;

			std::vector<std::string> character_name_buf_;  // Display name
			std::vector<std::string> character_inner_name_buf_;  // Bind a asset
			
			std::vector<std::string> ori_command_buf_;
            std::vector<std::string> command_buf_;
			std::vector<std::string> command_asset_path_buf_;
			std::vector<std::string> command_arguments_buf_;

            std::string string_buf;

			bool oriResolve(const std::string& line);
			bool transformOriCommand();
			void transformCharacterName();
		public:
			NormalLineResolution();
			bool resolve(const std::string& line)
			{
                if (oriResolve(line))
                {
					if (!transformOriCommand())
					{
						return false;
					}

                    transformCharacterName();
                    return true;
                }
                return false;
			}
            const std::string& getStringBuf()
            {
                return string_buf;
            }
			const std::vector<std::string>& getCharacterNameBuf()
			{
				return character_name_buf_;
			}
			const std::vector<std::string>& getCharacterInnerNameBuf()
            {
                return character_inner_name_buf_;
            }
            const std::vector<std::string>& getCommandBuf()
            {
                return command_buf_;
            }
            const std::vector<std::string>& getCommandAssetPathBuf()
            {
                return command_asset_path_buf_;
            }
            const std::vector<std::string>& getCommandArgumentsBuf()
            {
                return command_arguments_buf_;
            }
		};
	}
}
#endif // VISUALNOVEL_NORMALLINERESOLUTION_H
