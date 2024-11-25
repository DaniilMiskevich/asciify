#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <algorithm>
#include <functional>

#include "image/image.hpp"

class ConvolvedImage : public Image {
   public:
    ConvolvedImage(Color const *const data, Size const size)
    : _size(size), data(data) {}
    ConvolvedImage(ConvolvedImage const &other)
    : _size(other._size), data(new Color[_size.area()]()) {
        std::copy(
            other.data,
            other.data + _size.area(),
            const_cast<Color *>(data)
        );
    }

    ~ConvolvedImage() { delete[] data; }

    Size size(void) const override { return _size; }
    Color const &operator[](Pos pos) const override {
        if (pos.x >= _size.w) pos.x = _size.w - 1;
        if (pos.y >= _size.h) pos.y = _size.h - 1;
        return data[pos.x + pos.y * _size.w];
    }

   private:
    Size const _size;
    Color const *const data;
};

template <typename T, uint16_t W, uint16_t H>
struct ConvolutionKernel {
    ConvolutionKernel() = delete;

    constexpr ConvolutionKernel(T const (&matrix)[H][W]) {
        std::copy(&matrix[0][0], &matrix[0][0] + H * W, &this->matrix[0][0]);
    }

    constexpr ConvolutionKernel(
        std::function<T(uint16_t const x, uint16_t const y)> const &fn
    ) {
        for (letmut i = uint16_t(0); i < H; i++)
            for (letmut j = uint16_t(0); j < W; j++) matrix[i][j] = fn(i, j);
    }

    T matrix[W][H];

    ConvolvedImage operator*(Image const &other) const {
        let size = other.size();
        letmut data = new Color[size.area()];

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            Color src[H][W];
            for (letmut i = uint16_t(0); i < H; i++)
                for (letmut j = uint16_t(0); j < W; j++) {
                    src[i][j] =
                        other[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
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
                            other[pos + Pos(j - (W - 1) / 2, i - (H - 1) / 2)];
                    }
                }

                letmut s = Color();
                for (letmut i = uint16_t(0); i < H; i++)
                    for (letmut j = uint16_t(0); j < W; j++)
                        s += src[i][j] * matrix[H - 1 - i][W - 1 - j];

                data[pos.x + pos.y * size.w] = s;
            }
        }

        return ConvolvedImage(data, size);
    }
};

#endif
