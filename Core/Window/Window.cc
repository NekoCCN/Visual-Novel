#pragma execution_character_set("utf-8")
#include "Window.h"

using namespace vn;

core::Window::Window(const char* title, uint32_t width, uint32_t height, const char* drive, SDL_WindowFlags flags)
{
	// window part
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to create a window");
	window_ = SDL_CreateWindow(title, width, height, flags | SDL_WINDOW_HIDDEN);
	if (window_ == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window");
		throw exception::create_window_error();
	}

	// renderer part
	drive_num_ = SDL_GetNumRenderDrivers();
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Supported Graphic API List: ");
	for (int i = 0; i < drive_num_; ++i)
	{
		render_drive_str_.push_back(std::string(SDL_GetRenderDriver(i)));
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, render_drive_str_[i].c_str());
	}
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "END");
	sdl_renderer_ = SDL_CreateRenderer(window_, drive);
	if (sdl_renderer_ == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create graphic context");
		throw exception::create_window_error();
	}
	SDL_ShowWindow(window_);
}