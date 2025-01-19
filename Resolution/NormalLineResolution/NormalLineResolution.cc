#pragma execution_character_set("utf-8")
#include "NormalLineResolution.h"


bool vn::resolution::NormalLineResolution::resolve(const std::string& line)
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
	character_buf.clear();
	std::string::const_iterator character_it = buf.cbegin();
	while (regex_search(character_it, buf.cend(), res, plural_regex_))
	{
		character_buf.push_back(res[1]);
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
	command_buf.clear();
	std::string::const_iterator command_it = buf.cbegin();
	while (regex_search(command_it, buf.cend(), res, plural_regex_))
	{
		command_buf.push_back(res[1]);
		command_it = res.suffix().first;
	}

	return true;
}
