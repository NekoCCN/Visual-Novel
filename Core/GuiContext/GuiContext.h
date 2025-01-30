#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_GUICONTEXT_H
#define VISUALNOVEL_GUICONTEXT_H
#include <SDL3/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif
#include "../Window/Window.h"
#include <memory>

namespace vn
{
	namespace core
	{
		class GuiContext
		{
		private:
			ImGuiIO* io;

			char* font_data_ = nullptr;
			uint64_t font_size_;

			static float calculateScreenScale(const float screen_width, const float screen_height, const float base_font_size = 30.0f)
			{
				constexpr float reference_width = 1920.0f;
				constexpr float reference_height = 1080.0f;

				float screen_diagonal = sqrt(screen_width * screen_width + screen_height * screen_height);
				float reference_diagonal = sqrt(reference_width * reference_width + reference_height * reference_height);

				float scale = screen_diagonal / reference_diagonal;

				float final_scale = (scale * base_font_size) / 30.0f;

				constexpr float min_scale = 0.5f;
				constexpr float max_scale = 2.0f;
				return std::clamp(final_scale, min_scale, max_scale);
			}

		public:
			GuiContext(const std::shared_ptr<core::Window>& window, const std::shared_ptr<char>& font_data = nullptr,
				uint64_t font_size = 0, float size_pixels = 30.0f, float scale = 1.0f)
				: font_size_(font_size)
			{
				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to Creating GUI Context");
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				io = &ImGui::GetIO();
				io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
				io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GUI Theme : Light");
				ImGui::StyleColorsLight();

				ImGui_ImplSDL3_InitForSDLRenderer(window->getWindowHinding(), window->getRendererHinding());
				ImGui_ImplSDLRenderer3_Init(window->getRendererHinding());

				
				if (font_data != nullptr && font_size_ > 0)
				{
					font_data_ = new char[font_size];
                    memcpy(font_data_, font_data.get(), font_size);
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to Load Font");
					io->Fonts->AddFontFromMemoryTTF(font_data_, font_size_, size_pixels);
					ImGui::GetIO().FontGlobalScale = calculateScreenScale(window->getWindowRect().w, window->getWindowRect().h, size_pixels);
				}
			}
			ImGuiIO& getIOContext() const
			{
				return *io;
			}

			static void whenChangedWindowSize(const std::shared_ptr<core::Window>& window)
			{
				ImGui::GetIO().FontGlobalScale = calculateScreenScale(window->getWindowRect().w, window->getWindowRect().h);
			}
			~GuiContext()
			{
				// Notice : This code will clean up the memory pointed to by the font pointer
				ImGui_ImplSDLRenderer3_Shutdown();
				ImGui_ImplSDL3_Shutdown();
				ImGui::DestroyContext();
			}
		};
	}
}
#endif // VISUALNOVEL_GUICONTEXT_H
