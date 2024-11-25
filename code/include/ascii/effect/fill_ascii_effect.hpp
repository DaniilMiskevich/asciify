#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cstring>

#include "ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image.hpp"

class FillAsciiEffect : public AsciiEffect {
   public:
    FillAsciiEffect(char const *const palette = " .:-;=+*#B%@")
    : palette(palette), palette_len(strlen(palette)) {}

    void operator()(AsciiArt &dst) const override {
        let &image = dst.get_image();
        let size = dst.get_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let avg_col =
                image.get_avg_in_region(it.get_pos(), image.get_size() / size);

            let lum = avg_col.get_luminance();

            letmut i = size_t(lum * palette_len);
            if (i >= palette_len) i = palette_len - 1;

            (*it) = AsciiEl(palette[i]);
        }
    }

   private:
    char const *const palette;
    size_t const palette_len;
};

#endif
