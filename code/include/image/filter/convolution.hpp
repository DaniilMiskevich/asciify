#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <cstring>

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

template <typename T, size_t W, size_t H>
struct ConvolutionKernel {
   public:
    constexpr ConvolutionKernel(T const (&matrix)[W][H]) {
        memcpy(this->matrix, matrix, W * H);
    };

    ConvolvedImage operator*(Image const &other) const {
        let size = other.get_size();
        letmut data = new Color[size.get_area()];

        Color src[W][H];

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                for (letmut i = size_t(0); i < W; i++)
                    for (letmut j = size_t(0); j < H; j++)
                        src[i][j] =
                            other[pos + Pos(i - (W - 1) / 2, j - (H - 1) / 2)];
                // // when stepping downwards, no need to calculate the entire
                // // section, so
                // {
                //     // we're moving all rows but the last higher
                //     memmove(src[0], src[1], W * (H - 1));
                //
                //     // and only calculating the last
                //     for (letmut i = size_t(0); i < W; i++)
                //         src[i][H - 1] = other
                //                             [pos + Pos(i - (W - 1) / 2,
                //                                        H - 1 - (H - 1) / 2)]
                //                                 .get_magnitude();
                // }

                letmut s = Color();
                for (letmut i = uint16_t(0); i < W; i++)
                    for (letmut j = uint16_t(0); j < H; j++)
                        s += src[i][j] *
                             matrix[/* W - 1 - */ i][/* H - 1 - */ j];
                data[pos.x + size.w * pos.y] = s;
            }
        }

        return ConvolvedImage(data, size);
    }

   private:
    T matrix[W][H];
};

#endif
