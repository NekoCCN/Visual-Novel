#pragma execution_character_set("utf-8")
#include "NormalLineResolution.h"


vn::resolution::NormalLineResolution::NormalLineResolution(): normal_line_regex_(presets::normalLineRegexChecker())
{
	character_regex_ = std::regex(R"(\[(.*)\])");

	plural_regex_ = std::regex(R"([^\s,]+)");

	command_regex_ = std::regex(R"(\{([\d\w\s,]*)\})");

	string_regex_ = std::regex(R"(\]\s+(.*)\s+\{)");

	command_plural_regex_ = std::regex(R"((\w+)\[(.*)\]\((.*)\))");
}

bool vn::resolution::NormalLineResolution::oriResolve(const std::string& line)
{
	if (normal_line_regex_.checkString(line) == false)
	{
		return false;
	}

	std::smatch res;
	std::string buf;

	// Character
	if (std::regex_search(line, res, character_regex_))
	{
		buf = res[1];
	}
	else
	{
		return false;
	}
	character_inner_name_buf_.clear();
	std::string::const_iterator character_it = buf.cbegin();
	while (regex_search(character_it, buf.cend(), res, plural_regex_))
	{
		character_inner_name_buf_.push_back(res[0]);
		character_it = res.suffix().first;
	}

	// String
	if (!std::regex_search(line, res, string_regex_))
	{
        string_buf.clear();
	}
    else
    {
		string_buf = res[1];
    }

	// Command
	if (std::regex_search(line, res, command_regex_))
	{
		buf = res[1];
	}
	else
	{
		return false;
	}
	ori_command_buf_.clear();
	std::string::const_iterator command_it = buf.cbegin();
	while (regex_search(command_it, buf.cend(), res, plural_regex_))
	{
		ori_command_buf_.push_back(res[1]);
		command_it = res.suffix().first;
	}

	return true;
}

bool vn::resolution::NormalLineResolution::transformOriCommand()
{
	command_buf_.clear();
	command_arguments_buf_.clear();
	command_asset_path_buf_.clear();

	if (ori_command_buf_.empty())
	{
        return true;
	}

	std::smatch res;

	for (auto& x : ori_command_buf_)
	{
		if (!std::regex_search(x, res, command_plural_regex_))
		{
			return false;
		}
		if (res.size() != 4)
		{
			return false;
		}

		std::string buf;

		buf = res[1];
		if (!buf.empty())
		{
			command_buf_.push_back(buf);
		}

		buf = res[2];
		if (!buf.empty())
		{
			command_asset_path_buf_.push_back(buf);
		}

        buf = res[3];
        if (!buf.empty())
        {
            command_arguments_buf_.push_back(buf);
        }
	}

	return true;
}

void vn::resolution::NormalLineResolution::transformCharacterName()
{
	character_name_buf_.clear();

    for (auto& x : character_inner_name_buf_)
    {
		uint64_t index = x.find_first_of('.');
        if (index == std::string::npos)
        {
            character_name_buf_.push_back(x);
        }
        else
        {
            character_name_buf_.push_back(x.substr(0, index));
        }
    }
}
