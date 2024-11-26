#ifndef DOG_FILTER_HPP
#define DOG_FILTER_HPP

#include "image/filter/convolution.hpp"

template <uint16_t S>
struct GaussianKernel : public ConvolutionKernel<float, S, S> {
    GaussianKernel()
    : ConvolutionKernel<float, S, S>([](uint16_t const i, uint16_t const j) {
          let x = i - 0.5 * S, y = j - 0.5 * S;
          return 1.0 / (M_2_PI * S * S) * expf(-(x * x + y * y) / (2 * S * S));
      }) {}
};

static let g_s = GaussianKernel<7>();
static let g_ks = GaussianKernel<5>();

struct DoGFilter {
   public:
    DoGFilter(Image<Color> const &src, float const eps, float const p = 1)
    : image(src.size()) {
        let a = g_s * src, b = g_ks * src;
        for (letmut it = image.begin(); it != image.end(); it++) {
            static let bright = Color(0xFFFFFF), dim = Color(0x000000);

            let px = a[it.pos()] * (1 + p) - b[it.pos()] * p;
            image[it.pos()] = px.sqr_magnitude() > eps * eps ? bright : dim;
        }
    }

    Image<Color> image;
};

#endif
