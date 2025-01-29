#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_CORE_COMMANDLIST_H
#define VISUALNOVEL_CORE_COMMANDLIST_H
#include <map>
#include <string>

namespace vn
{
	namespace core
	{
		namespace command
		{
			enum CommandListEnum : uint8_t
			{
				play_sound,
			};
			class CommandList
			{
			private:
				std::map<std::string, CommandListEnum> command_list_;
			public:
				CommandList()
				{
					command_list_.insert(std::pair<std::string, CommandListEnum>("play_sound", play_sound));
				}

				bool resolve(const std::string& str, CommandListEnum& val)
				{
					if (command_list_.find(str) == command_list_.end())
					{
						return false;
					}
					val = command_list_.at(str);
					return true;
				}
			};
		}
	};
}
#endif