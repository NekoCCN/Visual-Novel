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
				static std::map<std::string, CommandListEnum> command_list_;
				static bool is_init_;
			public:
				CommandList()
				{
					if (is_init_ == 0)
					{
						command_list_.insert(std::pair<std::string, CommandListEnum>("play_sound", play_sound));
					}
				}
			};
		}
	};
}
#endif