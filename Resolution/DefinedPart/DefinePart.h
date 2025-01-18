#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_DEFINEDPART_H
#define VISUALNOVEL_DEFINEDPART_H
#include <SDL3/SDL.h>
#include <regex>
#include <string>
#include <map>
#include <cstdint>
#include <vector>
#include "../FormatChecker/FormatChecker.h"

namespace vn
{
	namespace resolution
	{
		class DefinedPart
		{
		private:
			RegexChecker define_line_checker;

			std::regex define_line_regex;

			typedef std::pair<std::vector<std::string>, uint32_t> animationPair;

			std::map<std::string, std::string> define_image_map;
			std::map<std::string, animationPair> define_animation_map;
		public:
			DefinedPart() : define_line_checker(presets::bootDefinedLineRegexChecker())
			{
				define_line_regex = R"([^\s:]+)";
			}
			bool resolve(SDL_IOStream* rstream, uint64_t& line_number);
		};
	}
}
#endif // VISUALNOVEL_DEFINELINE_H
