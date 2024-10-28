#ifndef COLOR_ASCII_EFFECT_HPP
#define COLOR_ASCII_EFFECT_HPP

#include <cmath>

#include "ascii/ascii_effect.hpp"
#include "conviniences.hpp"

class ColorAsciiEffect : public AsciiEffect {
   public:
    constexpr static Color const colors[] = {
        Color(0x000000),
        Color(0x800000),
        Color(0x008000),
        Color(0x808000),
        Color(0x000080),
        Color(0x800080),
        Color(0x008080),
        Color(0xc0c0c0),
    };

    ColorAsciiEffect(bool const do_use_rgb = false) : do_use_rgb(do_use_rgb) {}

    void operator()(AsciiArt &dst) const override {
        let &image = dst.get_image();
        let size = dst.get_size();

        for (letmut i = 0; i < size.w; i++) {
            for (letmut j = 0; j < size.h; j++) {
                let pos = Pos(i, j);

                let avg_col =
                    image.get_avg_in_region(pos, image.get_size() / size);

                let col_i = do_use_rgb ? 8 : [avg_col]() {
                    size_t closest_i;
                    for (letmut i = closest_i = size_t(1); i < lenof(colors);
                         i++) {
                        if (Color::distance(colors[i], avg_col) <=
                            Color::distance(colors[closest_i], avg_col))
                            closest_i = i;
                    }
                    return closest_i;
                }();

                dst[pos] = AsciiEl(
                    "\033[3" + std::to_string(col_i) + ";" +
                    (do_use_rgb ? ("2;" + col_to_ansi_repr(avg_col)) : "2") +

                    "m" +

                    dst[pos].data +

                    "\033[0m"
                );
            }
        }
    }

   private:
    bool const do_use_rgb;

    static std::string col_to_ansi_repr(Color const color) {
        return std::to_string(int(color.r)) + ";" +
               std::to_string(int(color.g)) + ";" +
               std::to_string(int(color.b));
    }
};

#endif
