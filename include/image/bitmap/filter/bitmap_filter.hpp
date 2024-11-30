#ifndef BITMAP_FILTER_HPP
#define BITMAP_FILTER_HPP

#include <functional>

#include "image/bitmap/bitmap.hpp"
#include "image/image_filter.hpp"

class BitmapFilter : public ImageFilter<Bitmap> {
   public:
    template <typename T, uint16_t W, uint16_t H>
    struct CnvlKernel;
};

template <typename T, uint16_t W, uint16_t H>
struct BitmapFilter::CnvlKernel {
    constexpr explicit CnvlKernel(T const (&matrix)[H][W]) {
        std::copy(&matrix[0][0], &matrix[0][0] + H * W, &this->matrix[0][0]);
    }

    constexpr explicit CnvlKernel(
        std::function<T(uint16_t const x, uint16_t const y)> const &fn
    ) {
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++) matrix[i][j] = fn(i, j);
    }

    T matrix[W][H];

    Image<Color> const &apply(Image<Color> &result, Image<Color> const &image)
        const {
        let size = result.size();

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            Color src[H][W];
            for (letmut i = uint16_t(0); i < H; i++)
                for (letmut j = uint16_t(0); j < W; j++) {
                    src[i][j] =
                        image[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
                }

            for (pos.y = 0; pos.y < size.h; pos.y++) {
                // stepping down, so need to calculate the entire section
                if (pos.y > 0) {
                    // just move each row up a row
                    for (letmut row = src; row < src + H - 1; row++)
                        std::move(&*(row + 1)[0], &*(row + 1)[0] + W, &*row[0]);

                    // and only calculate the last
                    let i = H - 1;
                    for (letmut j = uint16_t(0); j < W; j++) {
                        src[i][j] =
                            image[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
                    }
                }

                letmut s = Color();
                for (letmut i = uint16_t(0); i < H; i++)
                    for (letmut j = uint16_t(0); j < W; j++)
                        s += src[i][j] * matrix[H - 1 - i][W - 1 - j];

                result[pos] = s;
            }
        }

        return result;
    }
};

#endif
