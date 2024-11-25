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

        letmut pos = Pos(0, 0);
        // TODO iterator + algorithm
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                let avg_col =
                    image.get_avg_in_region(pos, image.get_size() / size);

                let lum = avg_col.get_luminance();

                letmut i = size_t(lum * palette_len);
                if (i >= palette_len) i = palette_len - 1;

                dst[pos] = AsciiEl(palette[i]);
            }
        }
    }

   private:
    char const *const palette;
    size_t const palette_len;
};

#endif
