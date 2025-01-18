#include "MainMenu.h"
#pragma execution_character_set("utf-8")

void vn::render::MainMenu::loadTextureToRect(const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs)
{
    // Reference to the Asset/ThemeAssetList/ThemeAssetList.h
    frect_buf_.clear();
    frect_buf_.push_back(rcs->constructRenderFRectByTexture_geometricScalingByH(texture_buf_[0], 0.25, 0.08, 0.11));
    frect_buf_.push_back(rcs->constructRenderFRectByTexture_geometricScalingByH(texture_buf_[1], 0.1, 0.1, 0.4));
    frect_buf_.push_back(rcs->constructRenderFRectByTexture_geometricScalingByH(texture_buf_[2], 0.1, 0.1, 0.53));
    frect_buf_.push_back(rcs->constructRenderFRectByTexture_geometricScalingByH(texture_buf_[3], 0.1, 0.1, 0.66));
    frect_buf_.push_back(rcs->constructRenderFRectByTexture_geometricScalingByH(texture_buf_[4], 0.1, 0.1, 0.79));
}

vn::render::MainMenu::MainMenu(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer,
                               const std::shared_ptr<core::Window>& window_pointer, 
                               const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs)
    : BasicRender(theme_pack_pointer, window_pointer)
{
	// Reference to the Asset/ThemeAssetList/ThemeAssetList.h
	for (int i = 1; i <= 5; ++i)
	{
		SDL_IOStream* rstream =  SDL_IOFromMem((*theme_pack_pointer_)[i].get(), theme_pack_pointer_->getFileSize(i));
		texture_buf_.push_back(IMG_LoadTexture_IO(window_pointer_->getRendererHinding(),
			rstream, true));
	}

    loadTextureToRect(rcs);
}

vn::render::MainMenu::~MainMenu()
{
    for (auto& texture : texture_buf_)
    {
        SDL_DestroyTexture(texture);
    }
}

void vn::render::MainMenu::whenChangedWindowSize(
	const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs)
{
    loadTextureToRect(rcs);
}

void vn::render::MainMenu::setThemePack(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer)
{
	theme_pack_pointer_ = theme_pack_pointer;
    for (auto& texture : texture_buf_)
    {
        SDL_DestroyTexture(texture);
    }

    for (int i = 1; i <= 5; ++i)
    {
        SDL_IOStream* rstream = SDL_IOFromMem((*theme_pack_pointer_)[i].get(), theme_pack_pointer_->getFileSize(i));
        texture_buf_.push_back(IMG_LoadTexture_IO(window_pointer_->getRendererHinding(),
            rstream, true));
    }
}

void vn::render::MainMenu::render()
{
    if (render_status_ == false)
    {
        return;
    }

    for (uint32_t i = 0; i < texture_buf_.size(); ++i)
    {
        window_pointer_->renderTexture(texture_buf_[i], nullptr, &frect_buf_[i]);
    }
}

void vn::render::MainMenu::response(SDL_Event* event, int32_t& status, uint32_t& button_click) const
{
    if (response_status_ == false)
    {
        return;
    }
    for (uint32_t i = 1; i < texture_buf_.size(); ++i)
    {
        if (universalMouseEventListen(event, MouseEvent::MOUSE_BUTTON_UP, frect_buf_[i]))
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Mouse button up : %d", i);
            button_click = i;
            if (i == 4)
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Normal close program", i);
                status = 0;
            }
            break;
        }
    }
}
