#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_LOGFUNCTION_H
#define VISUALNOVEL_LOGFUNCTION_H
#include <SDL3/SDL.h>

namespace vn
{
	namespace core
	{
		namespace log
		{
			void initLogSys();

			void setAllPriority(SDL_LogPriority priority);
		}
	}
}

#endif // VISUALNOVEL_LOGFUNCTION_H
