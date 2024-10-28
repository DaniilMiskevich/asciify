#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cstring>

#include "ascii/ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image.hpp"

class FillAsciiEffect : public AsciiEffect {
   public:
    FillAsciiEffect(char const *const palette = " .:-;=+*#B%@")
    : palette(palette), palette_len(strlen(palette)) {}

    void operator()(AsciiArt &dst) const override {
        let &image = dst.get_image();
        let size = dst.get_size();

        for (letmut i = 0; i < size.w; i++) {
            for (letmut j = 0; j < size.h; j++) {
                let pos = Pos(i, j);

                let avg_color =
                    image.get_avg_in_region(pos, image.get_size() / size);

                dst[pos] = AsciiEl(ascii_from_color(avg_color));
            }
        }
    }

   private:
    char const *const palette;
    size_t const palette_len;

    char ascii_from_color(Color const color) const {
        let lum = color.get_luminance() / 255.0;
        let ascii_idx =
            lum == 1.0 ? palette_len - 1 : size_t(lum * palette_len);
        return palette[ascii_idx];
    }
};

#endif
