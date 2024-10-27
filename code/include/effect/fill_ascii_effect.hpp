#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cmath>
#include <cstring>

#include "conviniences.hpp"
#include "dims.hpp"
#include "effect/ascii_art.hpp"
#include "effect/ascii_effect.hpp"
#include "image/image.hpp"

struct FillAsciiEffect : private AsciiEffect {
    FillAsciiEffect() : FillAsciiEffect(" .:-;=+*#B%@") {}

    FillAsciiEffect(char const *const pallette)
    : pallette(pallette), pallette_len(strlen(pallette)) {}

    void operator()(Image const &src, AsciiArt &tgt) const override {
        let size = tgt.get_size();
        for (letmut i = 0; i < size.w; i++) {
            for (letmut j = 0; j < size.h; j++) {
                letmut avg_r = 0.0, avg_g = 0.0, avg_b = 0.0;

                let char_size = tgt.get_char_size();
                for (letmut dx = 0; dx < char_size.w; dx++)
                    for (letmut dy = 0; dy < char_size.h; dy++) {
                        let pix = src
                            [Pos(i * char_size.w + dx, j * char_size.h + dy)];
                        avg_r += pix.r, avg_g += pix.g, avg_b += pix.b;
                    }

                let char_area = char_size.get_area();
                avg_r /= char_area, avg_g /= char_area, avg_b /= char_area;

                let avg_color = Image::Pixel(avg_r, avg_g, avg_b);

                tgt[Pos(i, j)] = AsciiArt::El(ascii_from_color(avg_color));
            }
        }
    }

   private:
    char const *const pallette;
    size_t const pallette_len;

    char ascii_from_color(Image::Pixel const color) const {
        let lum =
            (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 255.0;
        let ascii_idx =
            lum == 1.0 ? pallette_len - 1 : size_t(lum * pallette_len);
        return pallette[ascii_idx];
    }
};

#endif
