#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_CORE_WINDOW_H
#define VISUALNOVEL_CORE_WINDOW_H
#include <SDL3/SDL.h>
#include <Core/Monitor/Monitor.h>
#include <string>
#include <vector>

namespace vn
{
	namespace core
	{
		class Window
		{
		public:
			Window(const char* title, uint32_t width = 1280, uint32_t height = 720, const char* drive = nullptr, SDL_WindowFlags flags = SDL_WINDOW_HIDDEN);
			~Window()
			{
				SDL_DestroyRenderer(sdl_renderer_);
				SDL_DestroyWindow(window_);
			}
			bool showWindow() const
				// maybe thread safe
			{
				if (!SDL_ShowWindow(window_))
					return false;
				return true;
			}
			bool hideWindow() const
				// maybe thread safe
			{
				if (!SDL_HideWindow(window_))
					return false;
				return true;
			}
			bool presentRender() const
			{
				if (!SDL_RenderPresent(sdl_renderer_))
				{
					SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Can not render");
					return false;
				}
				return true;
			}
			SDL_Renderer* getRendererHinding() const
			{
				return sdl_renderer_;
			}
			SDL_Window* getWindowHinding() const
			{
				return window_;
			}
			bool setRenderVSync(int32_t vsync) const
			{
				if (!SDL_SetRenderVSync(sdl_renderer_, vsync))
				{
					SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Fail to set render vsync");
					return false;
				}
				return true;
			}
			void setWindowPosition(int32_t x = SDL_WINDOWPOS_CENTERED, int32_t y = SDL_WINDOWPOS_CENTERED) const
			{
				SDL_SetWindowPosition(window_, x, y);
			}
			bool isMinimized() const
			{
				return (SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED);
			}
			bool setRenderDrawColorInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
			{
				if (SDL_SetRenderDrawColor(sdl_renderer_, r, g, b, a))
					return true;
				return false;
			}

			bool setRenderDrawColorFloat(float r, float g, float b, float a) const
			{
				if (SDL_SetRenderDrawColorFloat(sdl_renderer_, r, g, b, a))
					return true;
				return false;
			}
			void renderClear() const
			{
				SDL_RenderClear(sdl_renderer_);
			}
			SDL_Rect getWindowRect() const
				// not include point, only have w,h
			{
				SDL_Rect tmp_rect{ 0, 0, 0, 0 };
				SDL_GetWindowSize(window_, &(tmp_rect.w), &(tmp_rect.h));
				return tmp_rect;
			}
			SDL_Surface* getWindowSurfaceCopy() const
			{
				SDL_Surface* current_surface = SDL_GetWindowSurface(window_);
				SDL_Surface* copy = SDL_CreateSurface(current_surface->w, current_surface->h, current_surface->format);
				SDL_BlitSurface(current_surface, nullptr, copy, nullptr);
				return copy;
			}
			SDL_Texture* surfaceToTexture(SDL_Surface* src) const
			{
				return SDL_CreateTextureFromSurface(sdl_renderer_, src);
			}
			void renderTexture(SDL_Texture* src, const SDL_FRect* src_rect = nullptr, const SDL_FRect* dst_rect = nullptr) const
			{
				SDL_RenderTexture(sdl_renderer_, src, src_rect, dst_rect);
			}
			void renderLine(float x1, float y1, float x2, float y2) const
			{
				SDL_RenderLine(sdl_renderer_, x1, y1, x2, y2);
			}
		private:
			// Surface-based software rendering is no longer used by default.
			// Software rendering cannot be used with hardware rendering
			std::vector<std::string> render_drive_str_;
			uint32_t drive_num_;
			SDL_Renderer* sdl_renderer_;
			SDL_Window* window_;
		};
	}
}
#endif