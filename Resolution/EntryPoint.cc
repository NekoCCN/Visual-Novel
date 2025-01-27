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
			}
		}
		else
		{
			if (!normal_line_resolution_.resolve(line))
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[Line: %ld] Invalid normal line format", line_number);
				log_window.log(SDL_LOG_PRIORITY_ERROR, "[Line: %ld] Invalid normal line format", line_number);
				return false;
			}

			asset::AssetPackWStream::ProgramIndex buf;

			
		}
	}

	return true;
}
