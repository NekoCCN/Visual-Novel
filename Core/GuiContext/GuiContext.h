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
			std::shared_ptr<char> font_data;
		public:
			GuiContext(const std::shared_ptr<core::Window>& window, const std::shared_ptr<char>& font_data = nullptr, uint64_t font_size = 0, float size_pixels = 15.0f)
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

				if (font_data != nullptr)
				{
					char* font_copy = new char[font_size];
					memcpy(font_copy, font_data.get(), font_size);
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to Load Font");
					io->Fonts->AddFontFromMemoryTTF(font_copy, font_size, 20.0f);
				}
			}
			ImGuiIO& getIOContext()
			{
				return *io;
			}
			~GuiContext()
			{
				ImGui_ImplSDLRenderer3_Shutdown();
				ImGui_ImplSDL3_Shutdown();
				ImGui::DestroyContext();
			}
		};
	}
}