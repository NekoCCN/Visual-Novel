#pragma execution_character_set("utf-8")
#include "LogFunction.h"

void vn::core::log::initLogSys()
{
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_COUNT, "[COUNT]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_CRITICAL, "[FATAL]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_DEBUG, "[DEBUG]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_ERROR, "[ERROR]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, "[INFO]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INVALID, "[INVALID]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_TRACE, "[TRACE]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_VERBOSE, "[VERBOSE]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_WARN, "[WARN]");
}
void vn::core::log::setAllPriority(SDL_LogPriority priority)
{
	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_ASSERT, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_AUDIO, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_ERROR, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_GPU, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_SYSTEM, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_TEST, priority);
	SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, priority);
}