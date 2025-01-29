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
            core::command::CommandList command_list_;

            NormalLineResolution normal_line_resolution_;
            DefinedPart defined_part_;

            std::string dst_path_;
            SDL_Storage* storage_;

            SDL_IOStream* rstream_;

            RegexChecker background_checker_;

            std::regex background_regex_{ R"(\((.*)\)##\((.*)\))" };

            bool getLine(std::string& buf) const;
        public:
            explicit EntryPoint(const std::string& src_path, const std::string& dst_path, SDL_Storage* storage = nullptr)
                : dst_path_(dst_path), storage_(storage), background_checker_(presets::backgroundLineRegexChecker())
            {
                rstream_ = SDL_IOFromFile(src_path.c_str(), "rb");
                if (rstream_ == nullptr)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open file: %s", SDL_GetError());
                    throw core::exception::file_not_found_error();
                }
            }

            bool operator()(render::LogWindow& log_window);

            ~EntryPoint()
            {
                if (rstream_)
                {
                    SDL_CloseIO(rstream_);
                }
            }
        };
	}
}

#endif // VISUAL_NOVEL_RESOLUTION_ENTRYPOINT_H
