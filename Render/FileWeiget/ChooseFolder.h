#pragma once
#ifndef VISUALNOVEL_CHOOSEFOLDER_H
#define VISUALNOVEL_CHOOSEFOLDER_H
#include <SDL3/SDL.h>
#include <string>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include "ImGuiFileDialog/ImGuiFileDialog.h"

namespace vn
{
	namespace render
	{
		class ChooseFile
		{
		private:
			bool render_status_ = false;
			std::string file_path_;
		public:
			void startRender()
			{
				render_status_ = true;
			}
			void renderAndResponse()
			{
				if (render_status_ == false)
				{
					return;
				}

				ImGui::Begin("Choose a Folder", &render_status_, ImGuiWindowFlags_NoCollapse);
				if (ImGui::Button("Browse"))
				{
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("MainChooseFile1", "Choose File", nullptr, config);
				}
				ImGui::InputText("File Path", &file_path_);
				if (ImGui::Button("OK"))
				{
					render_status_ = false;
				}

				if (ImGuiFileDialog::Instance()->Display("MainChooseFile1"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						file_path_ = ImGuiFileDialog::Instance()->GetCurrentPath();
					}

					ImGuiFileDialog::Instance()->Close();
				}

				ImGui::End();
			}
			const std::string& getFolderPath() const
			{
				return file_path_;
			}
		};
	}
}

#endif