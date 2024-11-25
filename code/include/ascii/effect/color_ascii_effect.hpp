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

        letmut pos = Pos(0, 0);
        // TODO iterator + algorithm
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                let avg_col =
                    image.get_avg_in_region(pos, image.get_size() / size);

                dst[pos].set_true_color(avg_col);
            }
        }
    }

   private:
};

#endif
