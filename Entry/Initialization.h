#pragma once
#ifndef VISUALNOVEL_INITIALIZATION_H
#define VISUALNOVEL_INITIALIZATION_H
#include <iostream>
#include <SDL3/SDL.h>
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogFunction.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace vn
{
	namespace entry
	{
		/*
        * Due to changes in the SDL_Image library, Webp format images will no longer be supported.
		*/
		class ProjectInitializer
		{
		private:
			static void init_exception_hinding()
			{
				set_terminate(core::exception::exception_hinding);
			}

			static void init_SDL()
			{
				if (!SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD))
				{
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window system initialization failed");
					throw core::exception::SDL_init_error();
				}
				else
				{
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window system initialization successful");
				}
			}

			static void init_SDL_TTF()
			{
				if (!TTF_Init())
				{
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TrueType system initialization failed");
					throw core::exception::SDL_init_error();
				}
				else
				{
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "TrueType system initialization successful");
				}
			}

			static void init()
			{
				init_exception_hinding();
				core::log::initLogSys();
				core::log::setAllPriority(SDL_LOG_PRIORITY_DEBUG);

				init_SDL();
				init_SDL_TTF();
			}
		public:
			ProjectInitializer()
			{
				init();
			}
			~ProjectInitializer()
			{
				TTF_Quit();
				SDL_Quit();
			}
		};
	}
}

#endif