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
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++)
                // transpose matrix for optimization
                this->matrix[j][i] = matrix[i][j];
    }

    constexpr explicit CnvlKernel(
        std::function<T(uint16_t const x, uint16_t const y)> const &fn
    ) {
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++) matrix[j][i] = fn(i, j);
    }

    T matrix[W][H];

    Image<Color> const &apply(Image<Color> &result, Image<Color> const &image)
        const {
        let size = result.size();

        letmut pos = Pos(0, 0);
        for (pos.y = 0; pos.y < size.h; pos.y++) {
            Color src[W][H];
            for (letmut i = uint16_t(0); i < W; i++)
                for (letmut j = uint16_t(0); j < H; j++) {
                    src[i][j] =
                        image[pos + Pos(i - (W - 1) / 2, j - (H - 1) / 2)];
                }

            for (pos.x = 0; pos.x < size.w; pos.x++) {
                // stepping down, no need to calculate the entire section
                if (pos.x > 0) {
                    // just move each col up a col
                    for (letmut col = src; col < src + W - 1; col++)
                        std::move(&col[1][0], &col[1][0] + H, &col[0][0]);

                    // and only calculate the last
                    let i = W - 1;
                    for (letmut j = uint16_t(0); j < H; j++) {
                        src[i][j] =
                            image[pos + Pos(i - (W - 1) / 2, j - (H - 1) / 2)];
                    }
                }

                letmut s = Color();
                for (letmut i = uint16_t(0); i < W; i++)
                    for (letmut j = uint16_t(0); j < H; j++)
                        s += src[i][j] * matrix[W - 1 - i][H - 1 - j];

                result[pos] = s;
            }
        }

        return result;
    }
};

#endif
