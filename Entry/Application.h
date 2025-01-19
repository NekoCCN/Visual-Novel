#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_APPLICATION_H
#define VISUALNOVEL_APPLICATION_H
#include "Initialization.h"
#include <SDL3/SDL.h>
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
#include <Core/GuiContext/GuiContext.h>
#include <Asset/ThemePack/ThemePack.h>
#include <Asset/AssetPackWStream/AssetPackWStream.h>
#include <Asset/ThemePackCreator/ThemePackCreator.h>
#include <Render/MainMenu/MainMenu.h>
#include <Render/FileWeiget/ChooseFile.h>
#include <Render/FileWeiget/ChooseVnap.h>
#include <Render/LogWindow/LogWindow.h>
#include <Resolution/DefinedPart/DefinePart.h>
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
			std::shared_ptr<render::ChooseFile> choose_file_;
			std::shared_ptr<render::ChooseVnap> choose_vnap_;
			std::shared_ptr<render::LogWindow> log_window_for_resolution;

			// Resolution part
			std::shared_ptr<resolution::DefinedPart> define_part_;

			// Response Part Function
			void Response_main_menu(SDL_Event* event, int32_t& status)
			{
				uint32_t click = 0;

				main_menu_->changeResponse(!(choose_file_->getRenderStatus() || choose_vnap_->getRenderStatus()));
				main_menu_->response(event, status, click);

				switch (click)
				{
				case 1:
					choose_file_->startRender();
					click = 0;
					break;
				case 2:
					choose_vnap_->startRender();
					click = 0;
					break;
				case 3:
				default:
					break;
				}

				if (choose_file_->isSuccess())
				{
					SDL_IOStream* rst = SDL_IOFromFile(choose_file_->getSrcFilePath().c_str(), "r");
					uint64_t line = 0;
					log_window_for_resolution->startRender();
					define_part_->resolve(rst, line, *log_window_for_resolution);
				}
			}
			
			// Render All Function
			void renderAll()
			{
				main_menu_->render();
			}
			void guiRenderAll()
			{
				choose_file_->renderAndResponse();
				choose_vnap_->renderAndResponse();
				log_window_for_resolution->renderAndResponse();
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
				gui_context_ = std::make_shared<core::GuiContext>(window_, (*theme_pack_)[0], theme_pack_->getFileSize(0),
					std::min(height, width) * 0.03);  // The min{height, width}*0.03 function to font pixel size seem work well

				// RatioCoordinateSystem
                ratio_coordinate_system_ = std::make_shared<core::coordinatesystem::StandardRatioCoordinateSystem<uint32_t>>(window_width_, window_height_);

				// Render Part
				main_menu_ = std::make_shared<render::MainMenu>(theme_pack_, window_, ratio_coordinate_system_);

				// GUI part
				choose_file_ = std::make_shared<render::ChooseFile>();
				choose_vnap_ = std::make_shared<render::ChooseVnap>();
				log_window_for_resolution = std::make_shared<render::LogWindow>("Resolution Log(.md)");

				// Resolution part
				define_part_ = std::make_shared<resolution::DefinedPart>();
			}
			int32_t operator()()
			{
				SDL_Event e;
				int32_t status = 1;

				window_->setRenderVSync(1);

				main_menu_->startRender();
				main_menu_->startResponse();

				while (status > 0)
				{
					while (SDL_PollEvent(&e))
					{
						ImGui_ImplSDL3_ProcessEvent(&e);

						if (e.type == SDL_EVENT_QUIT)
						{
							status = 0;
						}

						Response_main_menu(&e, status);
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
				return status;
			}

			~Application() = default;
		};
	}
}
#endif // VISUALNOVEL_APPLICATION_H
