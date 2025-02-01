#pragma once
#ifndef VISUALNOVEL_ENTRYMENU_H
#define VISUALNOVEL_ENTRYMENU_H
#pragma execution_character_set("utf-8")
#include <Render/MainMenu/MainMenu.h>
#include <Render/FileWeiget/ChooseFile.h>
#include <Render/FileWeiget/ChooseVnap.h>
#include <Render/LogWindow/LogWindow.h>
#include <Resolution/EntryPoint.h>
#include <Asset/AssetPackRStream/AssetPackRStream.h>
#include <future>
#include <chrono>
#include <thread>
#include <mutex>

namespace vn
{
	namespace entry
	{
		class EntryMenu
		{
		private:
			// Status
			bool is_active_;

			// Render Part
			std::shared_ptr<render::MainMenu> main_menu_;

			// GUI part
			std::shared_ptr<render::ChooseFile> choose_file_;
			std::shared_ptr<render::ChooseVnap> choose_vnap_;
			std::shared_ptr<render::LogWindow> log_window_for_resolution_;

			// Future
			std::future<bool> resolution_future_;

			// Resolution
			std::shared_ptr<resolution::EntryPoint> resolution_entry_;
			
			// Window Size Change
			bool window_size_changed_ = false;
			int32_t width_ = 0;
			int32_t height_ = 0;

			bool checkResolutionFutureReady() const
			{
                if (resolution_future_.valid())
				{
					if (resolution_future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
					{
						return true;
					}
					return false;
				}
				return true;
			}
		public:
			EntryMenu(const std::shared_ptr<asset::theme::ThemePack>& theme_pack, const std::shared_ptr<core::Window> window,
				const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs)
			{
				// Status
				is_active_ = false;

				// Render Part
				main_menu_ = std::make_shared<render::MainMenu>(theme_pack, window, rcs);

				// GUI part
				choose_file_ = std::make_shared<render::ChooseFile>();
				choose_vnap_ = std::make_shared<render::ChooseVnap>();
				log_window_for_resolution_ = std::make_shared<render::LogWindow>("Resolution Log(.md)");

				// Init
				main_menu_->startRender();
				main_menu_->startResponse();
			}
			void changeActive(bool status)
			{
				is_active_ = status;
			}
            bool isActive() const
			{
				return is_active_;
			}
			void Response_main_menu(SDL_Event* event, int32_t& status)
			{
				if (!checkResolutionFutureReady())
				{
					return;
				}

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
					window_size_changed_ = true;
					width_ = 1920 / 3;
					height_ = 1080 / 3;
				default:
					break;
				}

				if (choose_file_->isSuccess())
				{
					// You don't need to consider the life cycle of this pointer, resolution_entry_ will handle it.
					SDL_Storage* storage = SDL_OpenFileStorage(choose_file_->getFolderPath().c_str());
					resolution_entry_ = std::make_shared<resolution::EntryPoint>
						(choose_file_->getSrcFilePath(), choose_file_->getDstFilePath(), storage);
					resolution_future_ = std::async(std::launch::async, [&]() -> bool
						{
							return (*resolution_entry_)(*log_window_for_resolution_);
						});
					log_window_for_resolution_->startRender();
				}

				if (choose_vnap_->isSuccess())
				{
					asset::AssetPackRStream arst(choose_vnap_->getFilePath());
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", arst.getString(2).c_str());
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", arst.getString(4).c_str());
				}
			}
			SDL_Rect getWindowChangedRect()
			{
				if (!window_size_changed_)
				{
					window_size_changed_ = false;
					return { 0, 0, 0, 0 };
				}
				window_size_changed_ = false;
				return { 0, 0, width_, height_ };
			}
			void renderSDLPart() const
			{
				if (!is_active_)
				{
					return;
				}

				main_menu_->render();
			}
			void renderGUIPart() const
			{
				if (!is_active_)
				{
					return;
				}

				// GUI Part
				choose_file_->renderAndResponse();
				choose_vnap_->renderAndResponse();
				log_window_for_resolution_->renderAndResponse();
			}
			void whenChangedWindowSize(const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs) const
			{
				main_menu_->whenChangedWindowSize(rcs);
			}
		};
	}
}

#endif // VISUALNOVEL_MAINMENU_H