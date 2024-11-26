#ifndef DOG_IMAGE_FILTER_HPP
#define DOG_IMAGE_FILTER_HPP

#include "image/filter/image_cnvl_kernel.hpp"
#include "image/filter/image_filter.hpp"

template <uint16_t S>
struct GaussianKernel : public ImageCnvlKernel<float, S, S> {
    GaussianKernel()
    : ImageCnvlKernel<float, S, S>([](uint16_t const i, uint16_t const j) {
          let x = i - 0.5 * S, y = j - 0.5 * S;
          return 1.0 / (M_2_PI * S * S) * expf(-(x * x + y * y) / (2 * S * S));
      }) {}
};

class DoGImageFilter : public ImageFilter<Image<Color>> {
   public:
    DoGImageFilter(float const eps, float const p = 1) : eps(eps), p(p) {}

   private:
    float const eps, p;

    void operator()(Image<Color> &dst) const override {
        static let g_s = GaussianKernel<7>();
        static let g_ks = GaussianKernel<5>();

        let a = g_s * dst, b = g_ks * dst;
        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let px = a[it.pos()] * (1 + p) - b[it.pos()] * p;
            dst[it.pos()] = px.sqr_magnitude() > eps * eps ? Color(0xFFFFFF)
                                                           : Color(0x000000);
        }
    }
};

#endif
