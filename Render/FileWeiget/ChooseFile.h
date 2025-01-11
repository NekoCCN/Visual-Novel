#pragma once
#ifndef VISUALNOVEL_CHOOSEFILE_H
#define VISUALNOVEL_CHOOSEFILE_H
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <Core/CoordinateSystem/RatioCoordinateSystem.h>
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
			std::string src_path_;
			std::string folder_path_;
			std::string dst_path_;
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
				ImGui::SetNextWindowSize(ImVec2(0.0f, io.DisplaySize.y * 0.35));
				ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				ImGui::Begin("Choose a (.md) File and Path to Create File", &render_status_, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);

				if (ImGui::Button("Browse Source File"))
				{
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("MainChooseFile1", "File Selector", ".md", config);
				}
				ImGui::InputText("Source File Path", &src_path_);

				if (ImGui::Button("Browse Asset Folder"))
				{
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("MainChooseFolder1", "Folder Selector", nullptr, config);
				}
				ImGui::InputText("Asset(Image, Sound) Folder Path", &folder_path_);

				if (ImGui::Button("Browse Destination File"))
				{
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("MainChooseFile2", "File Selector", ".md", config);
				}
				ImGui::InputText("Destination File Path", &dst_path_);

				if (ImGui::Button("OK"))
				{
                    render_status_ = false;
				}

				if (ImGuiFileDialog::Instance()->Display("MainChooseFile1"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						src_path_ = ImGuiFileDialog::Instance()->GetCurrentPath();
					}

					ImGuiFileDialog::Instance()->Close();
				}

				if (ImGuiFileDialog::Instance()->Display("MainChooseFolder1"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						folder_path_ = ImGuiFileDialog::Instance()->GetCurrentPath();
					}

					ImGuiFileDialog::Instance()->Close();
				}

				if (ImGuiFileDialog::Instance()->Display("MainChooseFile2"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						dst_path_ = ImGuiFileDialog::Instance()->GetCurrentPath();
					}

					ImGuiFileDialog::Instance()->Close();
				}

				ImGui::End();
			}
			const std::string& getSrcFilePath() const
			{
				return src_path_;
			}
		};
	}
}
#endif // VISUALNOVEL_CHOOSEFILE_H
