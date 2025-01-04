#pragma once
#ifndef VISUALNOVEL_APPLICATION_H
#define VISUALNOVEL_APPLICATION_H
#include "Initialization.h"
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
#include <Asset/ThemePack/ThemePack.h>
#include <Asset/ThemePackCreator/ThemePackCreator.h>
#include <Render/MainMenu/MainMenu.h>
#include <memory>
#include <string>

namespace vn
{
	namespace entry
	{
		class Application
		{
		private:
			// Project Initializer
			ProjectInitializer project_initializer_;

			// Window and SDL_Renderer
			uint32_t window_width_;
			uint32_t window_height_;
			std::shared_ptr<core::Window> window_;

			// Should close flag
			bool should_close_ = false;

			// ThemePack
			std::shared_ptr<asset::theme::ThemePack> theme_pack_;

			// RatioCoordinateSystem
			std::shared_ptr<core::coordinatesystem::StandardRatioCoordinateSystem<uint32_t>> ratio_coordinate_system_;

			// Render Part
			std::shared_ptr<render::MainMenu> main_menu_;
		public:
			Application(const std::string& title = "NULL", uint32_t width = 1920, uint32_t height = 1080,
				const std::string& theme_pack_path = "basic_asset.vntp") :
					window_width_(width), window_height_(height)
			{
				window_ = std::make_shared<core::Window>(title.c_str(), width, height);
				theme_pack_ = std::make_shared<asset::theme::ThemePack>(theme_pack_path);
                ratio_coordinate_system_ = std::make_shared<core::coordinatesystem::StandardRatioCoordinateSystem<uint32_t>>(window_width_, window_height_);

				main_menu_ = std::make_shared<render::MainMenu>(theme_pack_, window_, ratio_coordinate_system_);
			}

			bool operator()()
			{
				SDL_Event e;
				bool quit = false;
				window_->setRenderVSync(1);

				main_menu_->startRender();
				main_menu_->startResponse();

				while (quit == false)
				{
					while (SDL_PollEvent(&e))
					{
						if (e.type == SDL_EVENT_QUIT)
							quit = true;
						main_menu_->response(&e, quit);
					}

					window_->setRenderDrawColorInt(255, 255, 255, 255);

					window_->renderClear();

					main_menu_->render();

					window_->presentRender();
				}
				return true;
			}
		};
	}
}
#endif // VISUALNOVEL_APPLICATION_H
