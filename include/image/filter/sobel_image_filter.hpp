#ifndef SOBEL_IMAGE_FILTER_HPP
#define SOBEL_IMAGE_FILTER_HPP

#include <thread>

#include "image/filter/image_cnvl_kernel.hpp"
#include "image/filter/image_filter.hpp"

class SobelImageFilter : public ImageFilter<Image<Color>> {
   public:
    SobelImageFilter() {}

   private:
    void operator()(Image<Color> &dst) const override {
        static let g_x = ImageCnvlKernel<float, 3, 3>({
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
        });
        static let g_y = ImageCnvlKernel<float, 3, 3>({
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        });

        letmut x = Image<Color>(dst.size()), y = Image<Color>(dst.size());
        letmut x_thread = std::thread([&x, &dst]() { g_x.apply(x, dst); }),
               y_thread = std::thread([&y, &dst]() { g_y.apply(y, dst); });
        x_thread.join();
        y_thread.join();

        for (letmut it = dst.begin(); it != dst.end(); it++)
            dst[it.pos()] = Color(x[it.pos()].sum(), y[it.pos()].sum(), 0);
    }
};

#endif
