#include "Monitor.h"

using namespace vn;

// Notice Exception Handling
SDL_Rect core::MonitorLists::getDisplayBound(int32_t index) const
{
	SDL_Rect tmp;
	if (!(index <= monitor_num_ - 1 && index >= 0))  //index from 0 to monitor num - 1
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"Invalid monitor index: %d < 0 or %d > monitor_num(%d) - 1", index, index, monitor_num_);
		throw exception::monitor_index_error();
	}
	SDL_GetDisplayBounds(monitor_list_[index], &tmp);
	return tmp;
}
// Notice Exception Handling
void core::MonitorLists::getDisplayBound(int32_t index, SDL_Rect* pt) const
{
	if (!(index <= monitor_num_ - 1 && index >= 0))  //index from 0 to monitor num - 1
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Invalid monitor index: %d < 0 or %d > monitor_num(%d) - 1", index, index, monitor_num_);
		throw exception::monitor_index_error();
	}
	SDL_GetDisplayBounds(monitor_list_[index], pt);
}
std::vector<std::string> core::MonitorLists::getMonitorNameList() const
{
	std::vector<std::string> tmp;
	for (int i = 0; i < monitor_num_; ++i)
		tmp.push_back(std::string(SDL_GetDisplayName(monitor_list_[i])));
	return tmp;
}
// Notice Exception Handling
SDL_DisplayID core::MonitorLists::operator[](int index) const
{
	if (!(index <= monitor_num_ - 1 && index >= 0))  //index from 0 to monitor num - 1
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Invalid monitor index: %d < 0 or %d > monitor_num(%d) - 1", index, index, monitor_num_);
		throw exception::monitor_index_error();
	}
	return monitor_list_[index];
}
