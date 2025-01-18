#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_BASICRENDER_H
#define VISUALNOVEL_BASICRENDER_H
#include <SDL3/SDL.h>
#include <memory>
#include <Asset/ThemePack/ThemePack.h>
#include <Core/Window/Window.h>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>

namespace vn
{
	namespace render
	{
		class BasicRender
		{
		protected:
			bool render_status_ = false;
			bool response_status_ = false;

			std::shared_ptr<asset::theme::ThemePack> theme_pack_pointer_;
			std::shared_ptr<core::Window> window_pointer_;
		public:
			BasicRender(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer, 
				const std::shared_ptr<core::Window>& window_pointer) :
				theme_pack_pointer_(theme_pack_pointer), window_pointer_(window_pointer)
			{  }
			BasicRender() = delete;
            BasicRender(BasicRender&& other) = delete;
            BasicRender(const BasicRender& other) = delete;
            BasicRender& operator=(BasicRender&& other) = delete;
            BasicRender& operator=(const BasicRender& other) = delete;

			virtual void setThemePack(const std::shared_ptr<asset::theme::ThemePack>& theme_pack_pointer) = 0;
			virtual void whenChangedWindowSize(const std::shared_ptr<core::coordinatesystem::RatioCoordinateSystem<uint32_t>>& rcs) = 0;
			virtual void render() = 0;

			virtual ~BasicRender() = default;

			// MouseEvent
			enum class MouseEvent : uint8_t
			{
				MOUSE_MOTION,
				MOUSE_BUTTON_DOWN,
				MOUSE_BUTTON_UP
			};
			static bool universalMouseEventListen(SDL_Event* event, MouseEvent event_type, const SDL_FRect& rect)
			{
				if (event_type == MouseEvent::MOUSE_BUTTON_DOWN)
				{
					if (event->type != SDL_EVENT_MOUSE_BUTTON_DOWN)
					{
						return false;
					}
				}
				else if (event_type == MouseEvent::MOUSE_BUTTON_UP)
				{
					if (event->type != SDL_EVENT_MOUSE_BUTTON_UP)
					{
						return false;
					}
				}
				else if (event_type == MouseEvent::MOUSE_MOTION)
				{
					if (event->type != SDL_EVENT_MOUSE_MOTION)
					{
						return false;
					}
				}

				float x, y;
				SDL_GetMouseState(&x, &y);


				bool flag = true;

				if (x < rect.x)
				{
					return false;
				}
				else if (x > rect.x + rect.w)
				{
					return false;
				}
				else if (y < rect.y)
				{
					return false;
				}
				else if (y > rect.y + rect.h)
				{
					return false;
				}

				return true;
			}

			void startRender()
			{
				render_status_ = true;
			}
			void stopRender()
			{
				render_status_ = false;
			}
			void changeRender(bool status)
			{
				render_status_ = status;
			}
			bool isRender() const
			{
				return render_status_;
			}
			void changeResponse(bool status)
			{
				response_status_ = status;
			}
			void startResponse()
			{
				response_status_ = true;
			}
			void stopResponse()
			{
				response_status_ = false;
			}
			bool isResponse() const
			{
				return response_status_;
			}
		};
	}
}

#endif // VISUALNOVEL_BASICRENDER_H