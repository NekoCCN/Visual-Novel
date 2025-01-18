#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_MAINMENU_H
#define VISUALNOVEL_MAINMENU_H
#include <SDL3/SDL.h>
#include <Render/BasicRender/BasicRender.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <vector>
#include <Asset/ThemePack/ThemePack.h>
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>

namespace vn
{
	namespace render
	{
		class MainMenu : public BasicRender
		{
		private:
			std::vector<SDL_Texture*> texture_buf_;
			std::vector<SDL_FRect> frect_buf_;

			void loadTextureToRect(const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs);
		public:
			MainMenu(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer,
				const std::shared_ptr<core::Window>& window_pointer, 
				const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs);
			MainMenu(const MainMenu& other) = delete;
            MainMenu(MainMenu&& other) = delete;
            MainMenu& operator=(const MainMenu& other) = delete;
			MainMenu& operator=(MainMenu&& other) = delete;

			void whenChangedWindowSize(const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs) override;
			void setThemePack(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer) override;
			void render() override;
			void response(SDL_Event* event, int32_t& status, uint32_t& button_click) const;

			~MainMenu() override;
		};
	}
}

#endif // VISUALNOVEL_MAINMENU_H