#ifndef SOBEL_FILTER_HPP
#define SOBEL_FILTER_HPP

#include "image/filter/convolution.hpp"
#include "image/filter/filter.hpp"

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

struct SobelFilter : Filter<Color> {
    void operator()(Image<Color> &dst) const override {
        let x = g_x * dst, y = g_y * dst;

        for (letmut it = dst.begin(); it != dst.end(); it++)
            dst[it.pos()] = Color(x[it.pos()].sum(), y[it.pos()].sum(), 0);
    }
};

#endif
