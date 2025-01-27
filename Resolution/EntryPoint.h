#pragma once
#ifndef VISUAL_NOVEL_RESOLUTION_ENTRYPOINT_H
#define VISUAL_NOVEL_RESOLUTION_ENTRYPOINT_H
#pragma execution_character_set("utf-8")
#include <SDL3/SDL_log.h>
#include <regex>
#include <Render/LogWindow/LogWindow.h>
#include "NormalLineResolution/NormalLineResolution.h"
#include "DefinedPart/DefinePart.h"
#include "FormatChecker/FormatChecker.h"
#include <Asset/AssetPackWStream/AssetPackWStream.h>

namespace vn
{
	namespace resolution
	{
        class EntryPoint
        {
        private:
            NormalLineResolution normal_line_resolution_;
            DefinedPart defined_part_;

            asset::AssetPackWStream wstream_;
            SDL_IOStream* rstream_;

            RegexChecker background_checker_;

            std::regex background_regex_{ R"(\((.*)\)##\((.*)\))" };

            bool getLine(std::string& buf) const;
        public:
            explicit EntryPoint(const std::string& src_path, const std::string& dst_path, SDL_Storage* storage = nullptr)
                : wstream_(dst_path, storage), background_checker_(presets::backgroundLineRegexChecker())
            {
                rstream_ = SDL_IOFromFile(src_path.c_str(), "rb");
            }
            bool operator()(render::LogWindow& log_window);
        };
	}
}

#endif // VISUAL_NOVEL_RESOLUTION_ENTRYPOINT_H
