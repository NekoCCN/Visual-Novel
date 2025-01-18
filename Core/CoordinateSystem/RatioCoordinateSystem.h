#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_RATIOCOORDINATESYSTEM_H
#define VISUALNOVEL_RATIOCOORDINATESYSTEM_H
#include <SDL3/SDL.h>
#include <Core/Window/Window.h>
#include <stdexcept>

namespace vn
{
	namespace core
	{
		namespace coordinatesystem
		{
			template <typename Tn> class RatioCoordinateSystem
			{
			protected:
				Tn width_;
				Tn height_;
			public:
				RatioCoordinateSystem() : width_(0), height_(0)
				{  }
				RatioCoordinateSystem(Tn width, Tn height) : width_(width), height_(height)
				{  }
                RatioCoordinateSystem(const RatioCoordinateSystem&) = default;
                RatioCoordinateSystem& operator=(const RatioCoordinateSystem&) = default;
                RatioCoordinateSystem(RatioCoordinateSystem&&) = default;
                RatioCoordinateSystem& operator=(RatioCoordinateSystem&&) = default;

				RatioCoordinateSystem(const Window& window)
				{
					SDL_Rect tmp = window.getWindowRect();
					width_ = tmp.w;
                    height_ = tmp.h;
				}
				Tn getW()
				{
					return width_;
				}
				Tn getH()
				{
					return height_;
				}
				void reset(Tn width, Tn height)
				{
					width_ = width;
                    height_ = height;
				}

				virtual SDL_Point operator()(double x_ratio, double y_ratio) = 0;
				virtual SDL_Rect createRect(double x_ratio, double y_ratio, double width_ratio, double height_ratio) = 0;
				virtual SDL_Rect createRectByTwoPoints(double x1_ratio, double y1_ratio, double x2_ratio, double y2_ratio) = 0;
				virtual SDL_FRect createFRect(double x_ratio, double y_ratio, double width_ratio, double height_ratio) = 0;
				virtual SDL_FRect createFRectByTwoPoints(double x1_ratio, double y1_ratio, double x2_ratio, double y2_ratio) = 0;

				static SDL_Rect geometricScalingRectByX(const SDL_Rect& rect, double dst_w)
				{
					double ratio = dst_w / rect.w;

					return {
						static_cast<int>(rect.x * ratio),
						static_cast<int>(rect.y * ratio),
						static_cast<int>(rect.w * ratio),
						static_cast<int>(rect.h * ratio)
					};
				}
				static SDL_FRect geometricScalingFRectByX(const SDL_FRect& rect, double dst_w)
				{
					double ratio = dst_w / rect.w;

					return {
						static_cast<float>(rect.x * ratio),
						static_cast<float>(rect.y * ratio),
						static_cast<float>(rect.w * ratio),
						static_cast<float>(rect.h * ratio)
					};
				}
				static SDL_Rect geometricScalingRectByY(const SDL_Rect& rect, double dst_h)
				{
					double ratio = dst_h / rect.h;

					return {
						static_cast<int>(rect.x * ratio),
						static_cast<int>(rect.y * ratio),
						static_cast<int>(rect.w * ratio),
						static_cast<int>(rect.h * ratio)
					};
				}
				static SDL_FRect geometricScalingFRectByY(const SDL_FRect& rect, double dst_h)
				{
					double ratio = dst_h / rect.h;

					return {
						static_cast<float>(rect.x * ratio),
						static_cast<float>(rect.y * ratio),
						static_cast<float>(rect.w * ratio),
						static_cast<float>(rect.h * ratio)
					};
				}

				virtual SDL_Rect constructRenderRectByTexture_geometricScalingByW(const SDL_Texture* texture, double occupied_w, double x_ratio, double y_ratio) = 0;
                virtual SDL_Rect constructRenderRectByTexture_geometricScalingByH(const SDL_Texture* texture, double occupied_h, double x_ratio, double y_ratio) = 0;
				virtual SDL_FRect constructRenderFRectByTexture_geometricScalingByW(const SDL_Texture* texture, double occupied_w, double x_ratio, double y_ratio) = 0;
				virtual SDL_FRect constructRenderFRectByTexture_geometricScalingByH(const SDL_Texture* texture, double occupied_h, double x_ratio, double y_ratio) = 0;

				float getRatioWidthFloatSize(double ratio)
				{
                    return static_cast<float>(width_ * ratio);
				}
				int getRatioWidthIntSize(double ratio)
				{
                    return static_cast<int>(width_ * ratio);
				}
                float getRatioHeightFloatSize(double ratio)
				{
                    return static_cast<float>(height_ * ratio);
				}
                int getRatioHeightIntSize(double ratio)
				{
                    return static_cast<int>(height_ * ratio);
				}

				virtual ~RatioCoordinateSystem() = default;
			};
			/*
            * SDL uses a different coordinate system:
            * The x axis still points to the right, but the y axis points down and the origin is in the top left.
			*/
			template <typename Tn> class StandardRatioCoordinateSystem : public RatioCoordinateSystem<Tn>
			{
			public:
				StandardRatioCoordinateSystem() = default;
                StandardRatioCoordinateSystem(Tn width, Tn height) : RatioCoordinateSystem<Tn>(width, height)
				{  }
                StandardRatioCoordinateSystem(const Window& window) : RatioCoordinateSystem<Tn>(window)
				{  }
                StandardRatioCoordinateSystem(const StandardRatioCoordinateSystem&) = default;
                StandardRatioCoordinateSystem& operator=(const StandardRatioCoordinateSystem&) = default;
                StandardRatioCoordinateSystem(StandardRatioCoordinateSystem&&) = default;
                StandardRatioCoordinateSystem& operator=(StandardRatioCoordinateSystem&&) = default;

                SDL_Point operator()(double x_ratio, double y_ratio) override
				{
					if (x_ratio < 0 || x_ratio > 1 || y_ratio < 0 || y_ratio > 1)
					{
						throw std::invalid_argument("RatioCoordinateSystem: x and y ratios must be between 0 and 1.");
					}

					return { static_cast<int>(x_ratio * this->width_), 
						static_cast<int>(y_ratio * this->height_) };
				}
				SDL_Rect createRect(double x_ratio, double y_ratio, double width_ratio, double height_ratio) override
				{
					if (x_ratio < 0 || x_ratio > 1 || y_ratio < 0
						|| y_ratio > 1 || width_ratio < 0 || width_ratio > 1 || height_ratio < 0 || height_ratio > 1)
					{
						throw std::invalid_argument("RatioCoordinateSystem: x, y, width and height ratios must be between 0 and 1.");
					}

					return {
						static_cast<int>(x_ratio * this->width_),
						static_cast<int>(y_ratio * this->height_),
						static_cast<int>(width_ratio * this->width_),
						static_cast<int>(height_ratio * this->height_) };
				}
				SDL_Rect createRectByTwoPoints(double x1_ratio, double y1_ratio, double x2_ratio, double y2_ratio) override
				{
                    if (x1_ratio < 0 || x1_ratio > 1 || y1_ratio < 0 || y1_ratio > 1 || x2_ratio < 0 || x2_ratio > 1 || y2_ratio < 0 || y2_ratio > 1)
					{
						throw std::invalid_argument("RatioCoordinateSystem: x and y ratios must be between 0 and 1.");
					}

                    return {
						static_cast<int>(std::min(x1_ratio, x2_ratio) * this->width_),
						static_cast<int>(std::min(y1_ratio, y2_ratio) * this->height_),
						static_cast<int>(std::abs(x1_ratio - x2_ratio) * this->width_),
						static_cast<int>(std::abs(y1_ratio - y2_ratio) * this->height_) };
				}
				SDL_FRect createFRect(double x_ratio, double y_ratio, double width_ratio, double height_ratio) override
				{
					if (x_ratio < 0 || x_ratio > 1 || y_ratio < 0
						|| y_ratio > 1 || width_ratio < 0 || width_ratio > 1 || height_ratio < 0 || height_ratio > 1)
					{
						throw std::invalid_argument("RatioCoordinateSystem: x, y, width and height ratios must be between 0 and 1.");
					}

                    return {
						static_cast<float>(x_ratio * this->width_),
						static_cast<float>(y_ratio * this->height_),
						static_cast<float>(width_ratio * this->width_),
						static_cast<float>(height_ratio * this->height_) };
				}
				SDL_FRect createFRectByTwoPoints(double x1_ratio, double y1_ratio, double x2_ratio, double y2_ratio) override
				{
                    if (x1_ratio < 0 || x1_ratio > 1 || y1_ratio < 0 || y1_ratio > 1 || x2_ratio < 0 || x2_ratio > 1 || y2_ratio < 0 || y2_ratio > 1)
					{
						throw std::invalid_argument("RatioCoordinateSystem: x and y ratios must be between 0 and 1.");
					}

					return {
						static_cast<float>(std::min(x1_ratio, x2_ratio) * this->width_),
						static_cast<float>(std::min(y1_ratio, y2_ratio) * this->height_),
						static_cast<float>(std::abs(x1_ratio - x2_ratio) * this->width_),
						static_cast<float>(std::abs(y1_ratio - y2_ratio) * this->height_) };
				}
				
				SDL_Rect constructRenderRectByTexture_geometricScalingByW(const SDL_Texture* texture, double occupied_w, double x_ratio, double y_ratio) override
				{
					double ratio = this->width_ * occupied_w / texture->w;

					return {
						static_cast<int>(x_ratio * this->width_),
						static_cast<int>(y_ratio * this->height_),
						static_cast<int>(ratio * texture->w),
						static_cast<int>(ratio * texture->h)
					};
				}
				SDL_Rect constructRenderRectByTexture_geometricScalingByH(const SDL_Texture* texture, double occupied_h, double x_ratio, double y_ratio) override
				{
					double ratio = this->height_ * occupied_h / texture->h;

					return {
						static_cast<int>(x_ratio * this->width_),
						static_cast<int>(y_ratio * this->height_),
						static_cast<int>(ratio * texture->w),
						static_cast<int>(ratio * texture->h)
					};
				}
				SDL_FRect constructRenderFRectByTexture_geometricScalingByW(const SDL_Texture* texture, double occupied_w, double x_ratio, double y_ratio) override
				{
					double ratio = this->width_ * occupied_w / texture->w;

					return {
						static_cast<float>(x_ratio * this->width_),
						static_cast<float>(y_ratio * this->height_),
						static_cast<float>(ratio * texture->w),
						static_cast<float>(ratio * texture->h)
					};
				}
				SDL_FRect constructRenderFRectByTexture_geometricScalingByH(const SDL_Texture* texture, double occupied_h, double x_ratio, double y_ratio) override
				{
					double ratio = this->height_ * occupied_h / texture->h;

					return {
						static_cast<float>(x_ratio * this->width_),
						static_cast<float>(y_ratio * this->height_),
						static_cast<float>(ratio * texture->w),
						static_cast<float>(ratio * texture->h)
					};
				}

				~StandardRatioCoordinateSystem() override = default;
			};
		}
	}
}
#endif // VISUALNOVEL_RATIOCOORDINATESYSTEM_H
