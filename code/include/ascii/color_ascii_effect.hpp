#ifndef COLOR_ASCII_EFFECT_HPP
#define COLOR_ASCII_EFFECT_HPP

#include <cmath>

#include "ascii/ascii_effect.hpp"

template <bool IS_TRUE_COLOR>
class ColorAsciiEffect : public AsciiEffect {
   public:
    ColorAsciiEffect() {}

    void operator()(AsciiArt &dst) const override;

   private:
};

#endif
