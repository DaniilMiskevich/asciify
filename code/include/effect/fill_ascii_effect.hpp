#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cmath>

#include "conviniences.hpp"
#include "dims.hpp"
#include "effect/ascii_art.hpp"
#include "effect/ascii_effect.hpp"
#include "image/image.hpp"

struct FillAsciiEffect : private AsciiEffect {
    void operator()(Image const &src, AsciiArt &tgt) const override {
        let size = tgt.get_size();
        for (letmut i = 0; i < size.w; i++) {
            for (letmut j = 0; j < size.h; j++) {
                letmut avg_r = 0.0, avg_g = 0.0, avg_b = 0.0;

                let char_size = tgt.get_char_size();
                for (letmut dx = 0; dx < char_size.w; dx++)
                    for (letmut dy = 0; dy < char_size.h; dy++) {
                        let pix = src.get_pixel(
                            Pos(i * char_size.w + dx, j * char_size.h + dy)
                        );
                        avg_r += pix.r, avg_g += pix.g, avg_b += pix.b;
                    }

                let char_area = char_size.get_area();
                avg_r /= char_area, avg_g /= char_area, avg_b /= char_area;

                let avg_color = Image::Pixel(avg_r, avg_g, avg_b);

                tgt.set_el(Pos(i, j), AsciiArt::El(avg_color));
            }
        }
    }
};

#endif
