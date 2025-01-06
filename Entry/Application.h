#pragma once
#ifndef VISUALNOVEL_APPLICATION_H
#define VISUALNOVEL_APPLICATION_H
#include "Initialization.h"
#include <SDL3/SDL.h>
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
#include <Core/GuiContext/GuiContext.h>
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

			// Gui
			std::shared_ptr <core::GuiContext> gui_context_;

			// Should close flag
			bool should_close_ = false;

			// ThemePack
			std::shared_ptr<asset::theme::ThemePack> theme_pack_;

			// RatioCoordinateSystem
			std::shared_ptr<core::coordinatesystem::StandardRatioCoordinateSystem<uint32_t>> ratio_coordinate_system_;

			// Render Part
			std::shared_ptr<render::MainMenu> main_menu_;

			// GUI part

			// Render All Function
			void renderAll()
			{
				main_menu_->render();
			}
			void guiRenderAll()
			{
				ImGui::ShowDemoWindow();
			}
		public:
			Application(const std::string& title = "NULL", uint32_t width = 1920, uint32_t height = 1080,
				const std::string& theme_pack_path = "basic_asset.vntp") :
					window_width_(width), window_height_(height)
			{
				// Window
				window_ = std::make_shared<core::Window>(title.c_str(), width, height);

				// ThemePack
				theme_pack_ = std::make_shared<asset::theme::ThemePack>(theme_pack_path);

				// GuiContext
				gui_context_ = std::make_shared<core::GuiContext>(window_, (*theme_pack_)[0], theme_pack_->getFileSize(0));

				// RatioCoordinateSystem
                ratio_coordinate_system_ = std::make_shared<core::coordinatesystem::StandardRatioCoordinateSystem<uint32_t>>(window_width_, window_height_);

				// Render Part
				main_menu_ = std::make_shared<render::MainMenu>(theme_pack_, window_, ratio_coordinate_system_);

				// GUI part

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
						ImGui_ImplSDL3_ProcessEvent(&e);

						if (e.type == SDL_EVENT_QUIT)
						{
							quit = true;
						}

						main_menu_->response(&e, quit);
					}

					window_->setRenderDrawColorInt(255, 255, 255, 255);
					window_->renderClear();

					renderAll();
					
					ImGui_ImplSDLRenderer3_NewFrame();
					ImGui_ImplSDL3_NewFrame();
					ImGui::NewFrame();

					guiRenderAll();

					ImGui::Render();

					ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), window_->getRendererHinding());
					window_->presentRender();
				}
				return true;
			}
		};
	}
}
#endif // VISUALNOVEL_APPLICATION_H
