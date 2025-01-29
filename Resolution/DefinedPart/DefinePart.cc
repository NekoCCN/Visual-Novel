#include "DefinePart.h"
#pragma execution_character_set("utf-8")

// Consider pure UTF-8 for now
bool vn::resolution::DefinedPart::getLine(SDL_IOStream* rstream, std::string& buf)
{
    buf.clear();
    unsigned char ch = '0';

    if (!SDL_ReadU8(rstream, &ch))
    {
        return false;
    }

    if (ch != u8'\r')
    {
        buf.push_back(ch);
    }

    while (ch != u8'\n')
    {
        if (!SDL_ReadU8(rstream, &ch))
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

vn::resolution::DefinedPart::DefinedPart(): define_line_checker(presets::bootDefinedLineRegexChecker())
{
	define_line_regex = R"([^\s]+)";
}

bool vn::resolution::DefinedPart::resolve(SDL_IOStream* rstream, uint64_t& line_number, render::LogWindow& log_window)
{
    clear();

	std::string buf;

    getLine(rstream, buf);
    ++line_number;
    if (!define_line_checker.checkString(buf))
    {
        log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid define line format", line_number);
        return false;
    }

    log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] Start resolution define part", line_number);

    std::vector<std::string> res_buf;
    std::smatch res;
    while (getLine(rstream, buf))
    {
        ++line_number;

        if (define_line_checker.checkString(buf))
        {
            log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] End define line part", line_number);
            return true;
        }

        std::string::const_iterator s_it = buf.cbegin();
        res_buf.clear();
        while (regex_search(s_it, buf.cend(), res, define_line_regex))
        {
            res_buf.push_back(res[0]);
            s_it = res.suffix().first;
        }

        res_buf[0].pop_back();
        if (res_buf.size() == 2)
        {
            if (define_image_map.contains(res_buf[0]) || define_animation_map.contains(res_buf[0]))
            {
                log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] %s has already defined", line_number, res_buf[0].c_str());
                return false;
            }
            define_image_map.emplace(std::make_pair(res_buf[0], res_buf[1]));
            log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] Define %s to image %s", line_number, res_buf[0].c_str(), res_buf[1].c_str());
        }
        else if (res_buf.size() >= 3)
        {
            if (define_image_map.contains(res_buf[0]) || define_animation_map.contains(res_buf[0]))
            {
                log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] %s has already defined", line_number, res_buf[0].c_str());
                return false;
            }
            uint32_t speed = 0;
            try {
                speed = std::stoi(res_buf[res_buf.size() - 1]);
            }
            catch (std::exception& e)
            {
                log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid define line format", line_number);
                return false;
            }
            define_animation_map.emplace(std::make_pair<std::string, AnimationPair>(std::string(res_buf[1]),
                std::make_pair(std::vector<std::string>(res_buf.begin() + 1, res_buf.end() - 1), speed)));

            log_window.log(SDL_LOG_PRIORITY_INFO, "[Line: %ld] Define %s to animation with speed %d", line_number, res_buf[0].c_str(), speed);
        }
        else
        {
            log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid define line format", line_number);
        }
    }

    return true;
}
