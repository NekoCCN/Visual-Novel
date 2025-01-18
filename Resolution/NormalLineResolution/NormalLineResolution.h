#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_NORMALLINERESOLUTION_H
#define VISUALNOVEL_NORMALLINERESOLUTION_H
#include <SDL3/SDL.h>
#include <regex>
#include <string>
#include <vector>
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

			std::regex plural_regex_ ;

			std::regex command_regex_;

			std::regex string_regex_;

			std::vector<std::string> character_buf;
            std::vector<std::string> command_buf;
            std::string string_buf;
		public:
			NormalLineResolution() : normal_line_regex_(presets::normalLineRegexChecker())
			{
				character_regex_ = std::regex(R"(\[(.*)\])");

				plural_regex_ = std::regex(R"([^\s,]+)");

				command_regex_ = std::regex(R"(\{([\d\w\s,]*)\})");

				string_regex_ = std::regex(R"(\]\s+(.*)\s+\{)");
			}
			bool resolve(const std::string& line);
			const std::vector<std::string>& getCharacterBuf()
			{
                return character_buf;
			}
			const std::vector<std::string>& getCommandBuf()
			{
                return command_buf;
			}
            const std::string& getStringBuf()
            {
                return string_buf;
            }
		};
	}
}
#endif // VISUALNOVEL_NORMALLINERESOLUTION_H
