#include "Transform.h"

using namespace vn;

// Reference : https://github.com/pygame-community/pygame-ce
void core::transform::box_blur(SDL_Surface* src, SDL_Surface* dst, int radius, bool repeat)
{
    Uint8* srcpx = (Uint8*)src->pixels;
    Uint8* dstpx = (Uint8*)dst->pixels;
    const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(src->format);
    Uint8 nb = pfd->bytes_per_pixel;
    int w = dst->w, h = dst->h;
    int dst_pitch = dst->pitch;
    int src_pitch = src->pitch;
    int i, x, y, color;
    Uint32* buf = new uint32_t[dst_pitch];
    Uint32* sum_v = new uint32_t[dst_pitch];
    Uint32* sum_h = new uint32_t[nb];

    memset(sum_v, 0, dst_pitch * sizeof(Uint32));
    for (y = 0; y <= radius; y++) {  // y-pre
        for (i = 0; i < dst_pitch; i++) {
            sum_v[i] += srcpx[src_pitch * y + i];
        }
    }
    if (repeat) {
        for (i = 0; i < dst_pitch; i++) {
            sum_v[i] += srcpx[i] * radius;
        }
    }
    for (y = 0; y < h; y++) {  // y
        for (i = 0; i < dst_pitch; i++) {
            buf[i] = sum_v[i] / (radius * 2 + 1);

            // update vertical sum
            if (y - radius >= 0) {
                sum_v[i] -= srcpx[src_pitch * (y - radius) + i];
            }
            else if (repeat) {
                sum_v[i] -= srcpx[i];
            }
            if (y + radius + 1 < h) {
                sum_v[i] += srcpx[src_pitch * (y + radius + 1) + i];
            }
            else if (repeat) {
                sum_v[i] += srcpx[src_pitch * (h - 1) + i];
            }
        }

        memset(sum_h, 0, nb * sizeof(Uint32));
        for (x = 0; x <= radius; x++) {  // x-pre
            for (color = 0; color < nb; color++) {
                sum_h[color] += buf[x * nb + color];
            }
        }
        if (repeat) {
            for (color = 0; color < nb; color++) {
                sum_h[color] += buf[color] * radius;
            }
        }
        for (x = 0; x < w; x++) {  // x
            for (color = 0; color < nb; color++) {
                dstpx[dst_pitch * y + nb * x + color] =
                    sum_h[color] / (radius * 2 + 1);

                // update horizontal sum
                if (x - radius >= 0) {
                    sum_h[color] -= buf[(x - radius) * nb + color];
                }
                else if (repeat) {
                    sum_h[color] -= buf[color];
                }
                if (x + radius + 1 < w) {
                    sum_h[color] += buf[(x + radius + 1) * nb + color];
                }
                else if (repeat) {
                    sum_h[color] += buf[(w - 1) * nb + color];
                }
            }
        }
    }

    delete[] buf;
    delete[] sum_v;
    delete[] sum_h;
}

void core::transform::gaussian_blur(SDL_Surface* src, SDL_Surface* dst, int sigma, bool repeat)
{
    Uint8* srcpx = (Uint8*)src->pixels;
    Uint8* dstpx = (Uint8*)dst->pixels;
    const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(src->format);
    Uint8 nb = pfd->bytes_per_pixel;
    int w = dst->w, h = dst->h;
    int dst_pitch = dst->pitch;
    int src_pitch = src->pitch;
    int i, j, x, y, color;
    int kernel_radius = sigma * 2;
    float* buf = new float[dst_pitch];
    float* buf2 = new float[dst_pitch];
    float* lut = new float[kernel_radius + 1];
    float lut_sum = 0.0;

    for (i = 0; i <= kernel_radius; i++) {  // init gaussian lut
        // Gaussian function
        lut[i] =
            expf(-powf((float)i, 2.0f) / (2.0f * powf((float)sigma, 2.0f)));
        lut_sum += lut[i] * 2;
    }
    lut_sum -= lut[0];
    for (i = 0; i <= kernel_radius; i++) {
        lut[i] /= lut_sum;
    }

    for (i = 0; i < dst_pitch; i++) {
        buf[i] = 0.0;
        buf2[i] = 0.0;
    }

    for (y = 0; y < h; y++) {
        for (j = -kernel_radius; j <= kernel_radius; j++) {
            for (i = 0; i < dst_pitch; i++) {
                if (y + j >= 0 && y + j < h) {
                    buf[i] +=
                        (float)srcpx[src_pitch * (y + j) + i] * lut[abs(j)];
                }
                else if (repeat) {
                    if (y + j < 0) {
                        buf[i] += (float)srcpx[i] * lut[abs(j)];
                    }
                    else {
                        buf[i] += (float)srcpx[src_pitch * (h - 1) + i] *
                            lut[abs(j)];
                    }
                }
            }
        }

        for (x = 0; x < w; x++) {
            for (j = -kernel_radius; j <= kernel_radius; j++) {
                for (color = 0; color < nb; color++) {
                    if (x + j >= 0 && x + j < w) {
                        buf2[nb * x + color] +=
                            buf[nb * (x + j) + color] * lut[abs(j)];
                    }
                    else if (repeat) {
                        if (x + j < 0) {
                            buf2[nb * x + color] += buf[color] * lut[abs(j)];
                        }
                        else {
                            buf2[nb * x + color] +=
                                buf[nb * (w - 1) + color] * lut[abs(j)];
                        }
                    }
                }
            }
        }
        for (i = 0; i < dst_pitch; i++) {
            dstpx[dst_pitch * y + i] = (Uint8)buf2[i];
            buf[i] = 0.0;
            buf2[i] = 0.0;
        }
    }

    delete[] buf;
    delete[] buf2;
    delete[] lut;
}
