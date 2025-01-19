#pragma once
#ifndef VISUALNOVEL_LOGWINDOW_H
#define VISUALNOVEL_LOGWINDOW_H
#include <imgui.h>
#include <SDL3/SDL_log.h>

namespace vn
{
    namespace render
    {
        // reference Imgui demo code
        class LogWindow
        {
        private:
            ImGuiTextBuffer buf_;
            ImGuiTextFilter filter_;
            ImVector<int> line_offsets_;
            bool auto_scroll_;
            bool is_active_;
            std::string title_;

            struct LogEntry
            {
                std::string text;
                SDL_LogPriority level;
                int line_start;
                int line_end;
            };

            std::vector<LogEntry> entries_;

            const char* prefix_to_level(SDL_LogPriority level)
            {
                switch (level)
                {
                case SDL_LOG_PRIORITY_VERBOSE:
                    return "[VERBOSE]";
                case SDL_LOG_PRIORITY_DEBUG:
                    return "[DEBUG]";
                case SDL_LOG_PRIORITY_INFO:
                    return "[INFO]";
                case SDL_LOG_PRIORITY_WARN:
                    return "[WARN]";
                case SDL_LOG_PRIORITY_ERROR:
                    return "[ERROR]";
                case SDL_LOG_PRIORITY_CRITICAL:
                    return "[CRITICAL]";
                default:
                    return "[NULL]";
                }
            }

            ImVec4 get_level_color(SDL_LogPriority level)
            {
                switch (level)
                {
                case SDL_LOG_PRIORITY_VERBOSE:
                    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // 灰色
                case SDL_LOG_PRIORITY_DEBUG:
                    return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // 绿色
                case SDL_LOG_PRIORITY_INFO:
                    return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  // 白色
                case SDL_LOG_PRIORITY_WARN:
                    return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // 黄色
                case SDL_LOG_PRIORITY_ERROR:
                    return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // 红色
                case SDL_LOG_PRIORITY_CRITICAL:
                    return ImVec4(1.0f, 0.0f, 1.0f, 1.0f);  // 紫色
                default:
                    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                }
            }

        public:
            LogWindow(std::string title) : auto_scroll_(true), is_active_(false), title_(std::move(title))
            {
                clear();
            }

            void log(SDL_LogPriority level, const char* fmt, ...) IM_FMTARGS(3)
            {
                char buf[1024];
                va_list args;
                va_start(args, fmt);
                vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
                va_end(args);

                int old_size = buf_.size();

                LogEntry entry;
                entry.level = level;
                entry.text = buf;
                entry.line_start = old_size;

                buf_.append(prefix_to_level(level));
                buf_.append(" ");
                buf_.append(buf);
                buf_.append("\n");

                entry.line_end = buf_.size();
                entries_.push_back(entry);

                line_offsets_.push_back(buf_.size());
            }

            void renderAndResponse()
            {
                if (is_active_ == false)
                {
                    return;
                }

                if (!ImGui::Begin(title_.c_str(), &is_active_))
                {
                    ImGui::End();
                    return;
                }

                bool clear_status = ImGui::Button("Clear");
                ImGui::SameLine();
                bool copy_status = ImGui::Button("Copy");
                ImGui::SameLine();
                ImGui::Checkbox("Auto-scroll", &auto_scroll_);
                ImGui::SameLine();
                filter_.Draw("Filter", -100.0f);

                ImGui::Separator();

                if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    if (clear_status)
                        clear();
                    if (copy_status)
                        ImGui::LogToClipboard();

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

                    if (filter_.IsActive())
                    {
                        for (const auto& entry : entries_)
                        {
                            const char* line_start = buf_.begin() + entry.line_start;
                            const char* line_end = buf_.begin() + entry.line_end;

                            if (filter_.PassFilter(line_start, line_end))
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, get_level_color(entry.level));
                                ImGui::TextUnformatted(line_start, line_end);
                                ImGui::PopStyleColor();
                            }
                        }
                    }
                    else
                    {
                        ImGuiListClipper clipper;
                        clipper.Begin(entries_.size());
                        while (clipper.Step())
                        {
                            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                            {
                                if (i < entries_.size())
                                {
                                    const auto& entry = entries_[i];
                                    const char* line_start = buf_.begin() + entry.line_start;
                                    const char* line_end = buf_.begin() + entry.line_end;

                                    ImGui::PushStyleColor(ImGuiCol_Text, get_level_color(entry.level));
                                    ImGui::TextUnformatted(line_start, line_end);
                                    ImGui::PopStyleColor();
                                }
                            }
                        }
                        clipper.End();
                    }
                    ImGui::PopStyleVar();

                    if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                        ImGui::SetScrollHereY(1.0f);
                }
                ImGui::EndChild();
                ImGui::End();
            }

            void clear()
            {
                buf_.clear();
                entries_.clear();
                line_offsets_.clear();
                line_offsets_.push_back(0);
            }

            void startRender()
            {
                is_active_ = true;
            }
        };
    }
}
#endif // VISUALNOVEL_LOGWINDOW_H
