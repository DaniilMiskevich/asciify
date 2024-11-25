#ifndef COLOR_ASCII_EFFECT_HPP
#define COLOR_ASCII_EFFECT_HPP

#include <cmath>

#include "ascii_effect.hpp"

class ColorAsciiEffect : public AsciiEffect {
   public:
    ColorAsciiEffect() {}

    void operator()(AsciiArt &dst) const override {
        let &image = dst.get_image();
        let size = dst.get_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let avg_col =
                image.get_avg_in_region(it.get_pos(), image.get_size() / size);

            it->set_true_color(avg_col);
        }
    }

   private:
};

#endif
