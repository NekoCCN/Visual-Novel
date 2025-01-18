#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_CORE_TRANSFORM_H
#define VISUALNOVEL_CORE_TRANSFORM_H
#include <SDL3/SDL.h>

namespace vn
{
	namespace core
	{
		namespace transform
		{
			void gaussian_blur(SDL_Surface* src, SDL_Surface* dst, int sigma, bool repeat);
			void box_blur(SDL_Surface* src, SDL_Surface* dst, int radius, bool repeat);
		}
	}
}
#endif // VISUALTEXT_CORE_TRANSFORM_TRANSFORM_H
