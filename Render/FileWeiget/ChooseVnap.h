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
		class ChooseVnap
		{
		private:
			bool render_status_ = false;
			std::string file_path_;
		public:
			void startRender()
			{
				render_status_ = true;
			}
			bool getRenderStatus() const
			{
				return render_status_;
			}
			void renderAndResponse()
			{
				if (render_status_ == false)
				{
					return;
				}

				ImGuiIO& io = ImGui::GetIO();
				ImGui::SetNextWindowSize(ImVec2(0.0f, io.DisplaySize.y * 0.2));
				ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				ImGui::Begin("Choose a vnap(.vnap) File", &render_status_, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);
				if (ImGui::Button("Browse"))
				{
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("MainChooseFile1", "Choose File", ".vnap", config);
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
			~ChooseVnap()
			{
                ImGuiFileDialog::Instance()->Close();
			}
		};
	}
}

#endif