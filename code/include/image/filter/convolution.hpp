#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <cstring>
#include <functional>

#include "image/image.hpp"

class ConvolvedImage : public Image {
   public:
    ConvolvedImage(Color const *const data, Size const size)
    : data(data), size(size) {}

    ~ConvolvedImage() { delete[] data; }

    Size get_size(void) const override { return size; }
    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color::hex(0x000000);
        return data[pos.x + size.w * pos.y];
    }

   private:
    Color const *const data;
    Size const size;
};

template <typename T, uint16_t W, uint16_t H>
struct ConvolutionKernel {
    ConvolutionKernel() = delete;

    constexpr ConvolutionKernel(
        T const (&matrix)[H][W],
        float const factor = 1
    ) {
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++)
                this->matrix[i][j] = matrix[i][j] * factor;
    }

    constexpr ConvolutionKernel(
        std::function<T(uint16_t const x, uint16_t const y)> fn
    ) {
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++) matrix[i][j] = fn(i, j);
    }

    T matrix[W][H];

    ConvolvedImage operator*(Image const &other) const {
        let size = other.get_size();
        letmut data = new Color[size.get_area()];

        Color src[H][W];

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (letmut i = uint16_t(0); i < H; i++) {
                for (letmut j = uint16_t(0); j < W; j++) {
                    src[i][j] =
                        other[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
                }
            }

            for (pos.y = 0; pos.y < size.h; pos.y++) {
                // stepping down, so need to calculate the entire section
                if (pos.y > 0) {
                    // move all but the first rows higher
                    memmove(
                        reinterpret_cast<void *>(src[0]),
                        reinterpret_cast<void const *>(src[1]),
                        (lenof(src) - 1) * elsizeof(src)
                    );
                    // only calculate the last
                    let i = H - 1;
                    for (letmut j = uint16_t(0); j < W; j++) {
                        src[i][j] =
                            other[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
                    }
                }

                letmut s = Color();
                for (letmut i = uint16_t(0); i < H; i++) {
                    for (letmut j = uint16_t(0); j < W; j++) {
                        s += src[i][j] * matrix[H - 1 - i][W - 1 - j];
                    }
                }
                data[pos.x + size.w * pos.y] = s;
            }
        }

        return ConvolvedImage(data, size);
    }
};

#endif
