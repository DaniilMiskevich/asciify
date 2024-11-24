#ifndef SOBEL_FILTERED_IMAGE_HPP
#define SOBEL_FILTERED_IMAGE_HPP

#include <climits>

#include "image/filter/convolution.hpp"
#include "image/image.hpp"

class SobelFilteredImage : public Image {
   public:
    SobelFilteredImage(Image const &src)
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
        src) {
        let size = x.get_size();
        letmut max_sqr_magnitude = -INFINITY, min_sqr_magnitude = INFINITY;

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                let x = this->x[pos].r, y = this->y[pos].r;
                let sqr_magnitude = x * x + y * y;

                if (sqr_magnitude > max_sqr_magnitude)
                    max_sqr_magnitude = sqr_magnitude;
                else if (sqr_magnitude < min_sqr_magnitude)
                    min_sqr_magnitude = sqr_magnitude;
            }
        }

        sqr_normalization_factor = max_sqr_magnitude - min_sqr_magnitude;
        sqr_normalization_offset = -min_sqr_magnitude;
    }

    Size get_size(void) const override { return x.get_size(); }
    Color operator[](Pos const pos) const override {
        return Color(
            this->x[pos].get_luminance(),
            this->y[pos].get_luminance(),
            0
        );
    }

   private:
    ConvolvedImage const x, y;
    float sqr_normalization_factor, sqr_normalization_offset;
};

#endif
