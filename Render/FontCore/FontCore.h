#pragma once
#ifndef VISUALNOVEL_FONTCORE_H
#define VISUALNOVEL_FONTCORE_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <chrono>
#include <Core/Window/Window.h>
#include <Asset/ThemePack/ThemePack.h>
#include <Asset/AssetPackRStream/AssetPackRStream.h>

namespace vn
{
    namespace render
    {
        class FontCore
        {
        private:
            struct Character
            {
                std::string utf8_char;

                float x;
                float y;

                bool is_visible;
                TTF_Text* text;

                uint32_t page;

                Character(const std::string& c)
                    : utf8_char(c), is_visible(false), text(nullptr), x(0), y(0), page(0)
                {  }
                Character(const Character& other) = delete;
                Character(Character&& other) noexcept
                {
                    utf8_char = other.utf8_char;
                    is_visible = other.is_visible;
                    text = other.text;
                    x = other.x;
                    y = other.y;
                    page = other.page;

                    other.text = nullptr;
                }
                Character& operator=(const Character& other) = delete;
                Character& operator=(Character&& other) noexcept
                {
                    utf8_char = other.utf8_char;
                    is_visible = other.is_visible;
                    text = other.text;
                    x = other.x;
                    y = other.y;
                    page = other.page;

                    other.text = nullptr;
                    return *this;
                }

                ~Character()
                {
                    if (text)
                    {
                        TTF_DestroyText(text);
                    }
                }
            };

            TTF_Font* font_;
            std::shared_ptr<char> font_data;
            SDL_IOStream* font_stream_;
            TTF_TextEngine* text_engine_;
            bool immediate_mode_;

            SDL_FRect render_bounds_;

            std::deque<Character> characters_;
            uint32_t max_page_ = 0;
            uint32_t current_page_ = 0;

            SDL_Color text_color_;

            float char_spacing_;
            float line_spacing_;

            float current_x_;
            float current_y_;

            std::chrono::steady_clock::time_point last_char_time_;
            float char_delay_ms_;

            bool is_success = false;

            bool initialize(float char_spacing_ratio = 0.12f, float line_spacing_ratio = 1.2f)
            {
                if (!font_)
                {
                    return false;
                }

                char_spacing_ = TTF_GetFontLineSkip(font_) * char_spacing_ratio;
                line_spacing_ = TTF_GetFontLineSkip(font_) * line_spacing_ratio;
                current_x_ = render_bounds_.x;
                current_y_ = render_bounds_.y;
                return true;
            }

            bool updateCharacterPosition(Character& ch)
            {
                if (!ch.text)
                {
                    return false;
                }

                int w, h;
                TTF_GetTextSize(ch.text, &w, &h);

                // Check if we need to wrap to next line
                if (ch.utf8_char == "\n")
                {
                    current_x_ = render_bounds_.x;
                    current_y_ += line_spacing_;
                    return false;
                }
                if (current_x_ + w > render_bounds_.x + render_bounds_.w)
                {
                    current_x_ = render_bounds_.x;
                    current_y_ += line_spacing_;
                }

                if (!characters_.empty())
                {
                    ch.page = characters_.back().page;
                }

                // Check if we're out of vertical bounds
                if (current_y_ + h > render_bounds_.y + render_bounds_.h)
                {
                    current_x_ = render_bounds_.x;
                    current_y_ = render_bounds_.y;
                    if (!characters_.empty())
                    {
                        ch.page += 1;
                    }
                }

                ch.x = current_x_;
                ch.y = current_y_;

                current_x_ += w + char_spacing_;

                return true;
            }

            FontCore(const core::Window& window, const std::string& path, float ptsize = 14)
                : immediate_mode_(false)
            {
                font_stream_ = SDL_IOFromFile(path.c_str(), "rb");
                font_ = TTF_OpenFontIO(font_stream_, true, ptsize);
                text_engine_ = TTF_CreateRendererTextEngine(window.getRendererHinding());

                initialize();
            }

            FontCore(const core::Window& window, const std::shared_ptr<char>& data, uint64_t size, float ptsize = 14)
                : immediate_mode_(false)
            {
                font_data = data;
                font_stream_ = SDL_IOFromMem(font_data.get(), size);
                font_ = TTF_OpenFontIO(font_stream_, true, ptsize);

                text_engine_ = TTF_CreateRendererTextEngine(window.getRendererHinding());

                initialize();
            }

            FontCore(const core::Window& window, const asset::AssetPackRStream& asset_pack, uint64_t index, float ptsize = 14)
                : immediate_mode_(false)
            {
                if (!asset_pack.getAssetPointer(index, font_data))
                {
                    font_ = nullptr;
                    font_stream_ = nullptr;
                    font_data = nullptr;
                    throw core::exception::asset_pack_data_error();
                }

                font_stream_ = SDL_IOFromMem(font_data.get(), asset_pack.getAssetSize(index));
                font_ = TTF_OpenFontIO(font_stream_, true, ptsize);
                text_engine_ = TTF_CreateRendererTextEngine(window.getRendererHinding());

                initialize();
            }

            FontCore(const core::Window& window, const asset::theme::ThemePack& theme_pack, uint64_t index, float ptsize = 14)
                : immediate_mode_(false)
            {
                font_data = theme_pack[index];
                font_stream_ = SDL_IOFromMem(font_data.get(), theme_pack.getFileSize(index));
                font_ = TTF_OpenFontIO(font_stream_, true, ptsize);
                text_engine_ = TTF_CreateRendererTextEngine(window.getRendererHinding());

                initialize();
            }

        public:
            static std::shared_ptr<FontCore> fromAssetPack(const core::Window& window, const asset::AssetPackRStream& asset_pack,
                uint64_t index, float ptsize = 14)
            {
                FontCore* font = new FontCore(window, asset_pack, index, ptsize);
                return std::shared_ptr<FontCore>(font);
            }

            static std::shared_ptr<FontCore> fromThemePack(const core::Window& window, const asset::theme::ThemePack& theme_pack,
                uint64_t index, float ptsize = 14)
            {
                FontCore* font = new FontCore(window, theme_pack, index, ptsize);
                return std::shared_ptr<FontCore>(font);
            }

            static std::shared_ptr<FontCore> fromFile(const core::Window& window, const std::string& path, float ptsize = 14)
            {
                FontCore* font = new FontCore(window, path, ptsize);
                return std::shared_ptr<FontCore>(font);
            }

            static std::shared_ptr<FontCore> fromMemory(const core::Window& window, const std::shared_ptr<char>& data,
                uint64_t size, float ptsize = 14)
            {
                FontCore* font = new FontCore(window, data, size, ptsize);
                return std::shared_ptr<FontCore>(font);
            }


            void setTextColor(const SDL_Color& color)
            {
                text_color_ = color;
            }

            void setRenderBounds(const SDL_FRect& bounds)
            {
                render_bounds_ = bounds;
                current_x_ = bounds.x;
                current_y_ = bounds.y;
            }

            void setCharDelay(float delay_ms)
            {
                char_delay_ms_ = delay_ms;
            }

            void set_immediate_mode(bool immediate)
            {
                immediate_mode_ = immediate;
            }

            void clearText()
            {
                characters_.clear();
                current_x_ = render_bounds_.x;
                current_y_ = render_bounds_.y;

                max_page_ = 0;
                current_page_ = 0;
            }

            void appendText(const std::string& text)
            {
                std::string utf8_char;
                const char* ptr = text.c_str();
                size_t text_len = text.length();
                size_t pos = 0;
                uint32_t local_max_page = 0;

                while (pos < text_len)
                {
                    size_t char_len = 1;

                    if ((text[pos] & 0xE0) == 0xC0)
                    {
                        char_len = 2;
                    }
                    else if ((text[pos] & 0xF0) == 0xE0)
                    {
                        char_len = 3;
                    }
                    else if ((text[pos] & 0xF8) == 0xF0)
                    {
                        char_len = 4;
                    }

                    utf8_char.assign(ptr + pos, char_len);

                    Character new_char(utf8_char);
                    new_char.text = TTF_CreateText(text_engine_, font_, utf8_char.c_str(), utf8_char.size() + 1);

                    if (new_char.text)
                    {
                        if (updateCharacterPosition(new_char))
                        {
                            new_char.is_visible = immediate_mode_;

                            TTF_SetTextColor(new_char.text, text_color_.r, text_color_.g, text_color_.b, text_color_.a);
                            local_max_page = std::max(local_max_page, new_char.page);
                            characters_.push_back(std::move(new_char));
                        }
                    }
                    pos += char_len;
                }

                last_char_time_ = std::chrono::steady_clock::now();

                max_page_ = std::max(max_page_, local_max_page);
            }

            uint32_t getPageCount() const
            {
                return max_page_ + 1;
            }

            void setSpacingRatio(float char_spacing_ratio = 0.12f, float line_spacing_ratio = 1.2f)
            {
                initialize(char_spacing_ratio, line_spacing_ratio);
            }

            uint32_t getCurrentPage() const
            {
                return current_page_;
            }

            void setPage(uint32_t page)
            {
                if (page <= max_page_)
                {
                    current_page_ = page;
                }
            }

            void nextPage()
            {
                if (current_page_ < max_page_)
                {
                    current_page_++;
                }
            }

            void prevPage()
            {
                if (current_page_ > 0)
                {
                    current_page_--;
                }
            }

            void update()
            {
                if (immediate_mode_)
                {
                    return;
                }

                auto current_time = std::chrono::steady_clock::now();
                float elapsed = std::chrono::duration<float, std::milli>(current_time - last_char_time_).count();

                for (auto& ch : characters_)
                {
                    if (!ch.is_visible && elapsed >= char_delay_ms_ && ch.page == current_page_) // Only update visibility on current page
                    {
                        ch.is_visible = true;
                        last_char_time_ = current_time;
                        break; // Important: Only reveal one character per update to respect delay
                    }
                }
            }

            void render() const
            {
                for (const auto& ch : characters_)
                {
                    if (ch.is_visible && ch.text && ch.page == current_page_) // Render only visible characters on current page
                    {
                        TTF_DrawRendererText(ch.text, ch.x, ch.y);
                    }
                }
            }

            bool isLastPage() const
            {
                return current_page_ == max_page_;
            }

            bool isSuccess() const
            {
                return current_page_ == max_page_ && characters_.back().is_visible == true;
            }

            void showAllCharactersOnCurrentPage()
            {
                for (auto& ch : characters_)
                {
                    if (ch.page == current_page_)
                    {
                        ch.is_visible = true;
                    }
                }
            }
        };
    }
}

#endif // VISUALNOVEL_FONTCORE_H