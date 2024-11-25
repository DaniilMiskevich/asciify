#ifndef SOBEL_FILTER_HPP
#define SOBEL_FILTER_HPP

#include "image/filter/convolution.hpp"

class SobelFilter : public Image {
   public:
    SobelFilter(Image const &src)
    : x(ConvolutionKernel<float, 3, 3>({
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
        }) *
        src),
      y(ConvolutionKernel<float, 3, 3>({
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        }) *
        src) {}

    Size get_size(void) const override { return x.get_size(); }

    // x and y gradienst are stored in r and g channels respectfully
    Color operator[](Pos const pos) const override {
        return Color(x[pos].get_sum(), y[pos].get_sum(), 0);
    }

   private:
    ConvolvedImage const x, y;
};

#endif
