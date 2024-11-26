#ifndef SOBEL_FILTER_HPP
#define SOBEL_FILTER_HPP

#include "image/filter/convolution.hpp"

static let g_x = ConvolutionKernel<float, 3, 3>({
    {+1, +0, -1},
    {+2, +0, -2},
    {+1, +0, -1},
});
static let g_y = ConvolutionKernel<float, 3, 3>({
    {+1, +2, +1},
    {+0, +0, +0},
    {-1, -2, -1},
});

struct SobelFilter {
   public:
    SobelFilter(Image const &src) : image(src.size()) {
        let x = g_x * src, y = g_y * src;

        for (letmut it = image.begin(); it != image.end(); it++)
            image[it.pos()] = Color(x[it.pos()].sum(), y[it.pos()].sum(), 0);
    }

    Image image;
};

#endif
