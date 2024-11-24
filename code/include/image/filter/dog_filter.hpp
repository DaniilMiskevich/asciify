#ifndef DOG_FILTER_HPP
#define DOG_FILTER_HPP

#include "image/filter/convolution.hpp"

template <uint16_t S>
struct GaussianKernel : public ConvolutionKernel<float, S, S> {
    GaussianKernel()
    : ConvolutionKernel<float, S, S>([](uint16_t const i, uint16_t const j) {
          let x = i - 0.5 * S, y = j - 0.5 * S;
          return 1. / (M_2_PI * S * S) * expf(-(x * x + y * y) / (2 * S * S));
      }) {}
};

class DoGFilter : public Image {
   public:
    DoGFilter(Image const &src, float const eps, float const p = 1)
    : a(GaussianKernel<7>() * src),
      b(GaussianKernel<5>() * src),
      eps(eps),
      p(p) {}

    Size get_size(void) const override { return a.get_size(); }

    // x and y gradienst are stored in r and g channels respectfully
    Color operator[](Pos const pos) const override {
        let pix = this->a[pos] * (1 + p) - this->b[pos] * p;

        return pix.get_sqr_magnitude() > eps * eps ? Color(1, 1, 1) : Color();
    }

   private:
    ConvolvedImage const a, b;
    float const eps, p;
};

#endif
