#ifndef COLOR_ASCII_EFFECT_HPP
#define COLOR_ASCII_EFFECT_HPP

#include <cmath>

#include "conviniences.hpp"
#include "dims.hpp"
#include "effect/ascii_art.hpp"
#include "effect/ascii_effect.hpp"
#include "image/image.hpp"

class ColorAsciiEffect : private AsciiEffect {
    AsciiArt::El operator()(Pos const pos, Size const size, Image const &src)
        const override {
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

        let current_c = (*prev)[pos];

        return AsciiArt::El(
            "\033[38;2;" + std::to_string(int(avg_color.r)) + ";" +
            std::to_string(int(avg_color.g)) + ";" +
            std::to_string(int(avg_color.b)) + "m" + current_c.data + "\033[0m"
        );
    }
};

#endif
