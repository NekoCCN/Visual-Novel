#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_APPLICATION_H
#define VISUALNOVEL_APPLICATION_H
#include "Initialization.h"
#include <SDL3/SDL.h>
#include "EntryMenu/EntryMenu.h"
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
#include <Core/GuiContext/GuiContext.h>
#include <Core/SpscQueue/SpscQueue.h>
#include <Asset/ThemePack/ThemePack.h>
#include <Asset/AssetPackRStream/AssetPackRStream.h>
#include <Asset/ThemePackCreator/ThemePackCreator.h>
#include <Render/MainMenu/MainMenu.h>
#include <Render/FileWeiget/ChooseFile.h>
#include <Render/FileWeiget/ChooseVnap.h>
#include <Render/LogWindow/LogWindow.h>
#include <Resolution/DefinedPart/DefinePart.h>
#include <Resolution/EntryPoint.h>
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

			// UI
			std::shared_ptr<EntryMenu> entry_menu_;

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

				// UI
                entry_menu_ = std::make_shared<EntryMenu>(theme_pack_, window_, ratio_coordinate_system_);
			}
			int32_t operator()()
			{
				SDL_Event e;
				int32_t status = 1;

				window_->setRenderVSync(1);

				entry_menu_->changeActive(true);

				while (status > 0)
				{
					while (SDL_PollEvent(&e))
					{
						ImGui_ImplSDL3_ProcessEvent(&e);

						if (e.type == SDL_EVENT_QUIT)
						{
							status = 0;
						}

						entry_menu_->Response_main_menu(&e, status);
					}

					window_->setRenderDrawColorInt(255, 255, 255, 255);
					window_->renderClear();

					entry_menu_->renderSDLPart();
					
					ImGui_ImplSDLRenderer3_NewFrame();
					ImGui_ImplSDL3_NewFrame();
					ImGui::NewFrame();

					entry_menu_->renderGUIPart();

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
