#ifndef SOBEL_BITMAP_FILTER_HPP
#define SOBEL_BITMAP_FILTER_HPP

#include <thread>

#include "image/bitmap/filter/bitmap_filter.hpp"

class SobelBitmapFilter : public BitmapFilter {
   private:
    void operator()(Bitmap &dst) const override {
        static let g_x = CnvlKernel<float, 3, 3>({
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
        });
        static let g_y = CnvlKernel<float, 3, 3>({
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        });

        letmut x = Bitmap(dst.size()), y = Bitmap(dst.size());
        letmut x_thread = std::thread([&x, &dst]() { g_x.apply(x, dst); }),
               y_thread = std::thread([&y, &dst]() { g_y.apply(y, dst); });
        x_thread.join();
        y_thread.join();

        for (letmut it = dst.begin(); it != dst.end(); it++)
            dst[it.pos()] = Color(x[it.pos()].sum(), y[it.pos()].sum(), 0);
    }
};

#endif
