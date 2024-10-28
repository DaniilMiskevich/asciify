#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cmath>
#include <cstring>

#include "conviniences.hpp"
#include "dims.hpp"
#include "effect/ascii_art.hpp"
#include "effect/ascii_effect.hpp"
#include "image/image.hpp"

class FillAsciiEffect : private AsciiEffect {
   public:
    FillAsciiEffect(
        AsciiEffect const &prev,
        char const *const palette = " .:-;=+*#B%@"
    )
    : AsciiEffect(prev), palette(palette), palette_len(strlen(palette)) {}

    Size get_size() const override { return prev->get_size(); }
    AsciiArt::El operator[](Pos const pos) const override {
        let size = prev->get_size();

        letmut avg_r = 0.0, avg_g = 0.0, avg_b = 0.0;

        let char_size = get_src().get_size() / size;
        for (letmut dx = 0; dx < char_size.w; dx++)
            for (letmut dy = 0; dy < char_size.h; dy++) {
                let pix = get_src(
                )[Pos(pos.x * char_size.w + dx, pos.y * char_size.h + dy)];
                avg_r += pix.r, avg_g += pix.g, avg_b += pix.b;
            }

        let char_area = char_size.get_area();
        avg_r /= char_area, avg_g /= char_area, avg_b /= char_area;

        let avg_color = Image::Pixel(avg_r, avg_g, avg_b);

        return AsciiArt::El(ascii_from_color(avg_color));
    }

   private:
    char const *const palette;
    size_t const palette_len;

    char ascii_from_color(Image::Pixel const color) const {
        let lum =
            (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 255.0;
        let ascii_idx =
            lum == 1.0 ? palette_len - 1 : size_t(lum * palette_len);
        return palette[ascii_idx];
    }
};

#endif
