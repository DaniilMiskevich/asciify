#ifndef SOBEL_FILTER_HPP
#define SOBEL_FILTER_HPP

#include "image/filter/convolution.hpp"

static let g_x = ConvolutionKernel<float, 3, 3>({
               {+1, +0, -1},
               {+2, +0, -2},
               {+1, +0, -1},
           }),
           g_y = ConvolutionKernel<float, 3, 3>({
               {+1, +2, +1},
               {+0, +0, +0},
               {-1, -2, -1},
           });

class SobelFilter : public Image {
   public:
    SobelFilter(Image const &src)
    : _size(src.size()), data(new Color[_size.area()]()) {
        let size = src.size();

        let x = g_x * src, y = g_y * src;

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++)
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                data[pos.x + pos.y * size.w] =
                    Color(x[pos].sum(), y[pos].sum(), 0);
            }
    }

    SobelFilter(SobelFilter const &other)
    : _size(other._size), data(new Color[_size.area()]()) {
        std::copy(other.data, other.data + _size.area(), data);
    }

    ~SobelFilter() { delete[] data; }

    Size size(void) const override { return _size; }

    // x and y gradienst are stored in r and g channels respectfully
    Color const &operator[](Pos const pos) const override {
        return data[pos.x + pos.y * _size.w];
    }

   private:
    Size const _size;
    // TODO!!! temprorary hack remove later
    Color *const data;
};

#endif
