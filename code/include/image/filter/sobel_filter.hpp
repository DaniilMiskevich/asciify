#ifndef SOBEL_FILTER_HPP
#define SOBEL_FILTER_HPP

#include "image/filter/convolution.hpp"
#include "image/image.hpp"

class SobelFilteredImage : public Image {
   public:
    SobelFilteredImage(Image const &src)
    : x(ConvolutionKernel<double, 3, 3>({
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
            // {1, 4, 7, 4, 1},
            // {4, 16, 26, 16, 4},
            // {7, 26, 41, 26, 7},
            // {4, 16, 26, 16, 4},
            // {1, 4, 7, 4, 1},
        }) *
        src),
      y(ConvolutionKernel<double, 3, 3>({
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        }) *
        src) {}

    Size get_size(void) const override { return x.get_size(); }
    Color operator[](Pos const pos) const override {
        let x = this->x[pos].r;
        let y = this->y[pos].r;
        let g = sqrt(x * x + y * y);
        if (g > 1)
            return Color(1, 1, 1);
        else
            return Color(0, 0, 0);
        return Color(g, g, g);
    }

   private:
    ConvolvedImage const x, y;
};

#endif
