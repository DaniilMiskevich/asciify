#ifndef ASCII_EL_HPP
#define ASCII_EL_HPP

#include <algorithm>
#include <iterator>

#include "color.hpp"

struct AsciiEl {
    AsciiEl() : AsciiEl(' ') {}

    explicit AsciiEl(char const c, Color const &color = Color(0xFFFFFF))
    : c(c), true_color(color), indexed_color_index(color_to_indexed(color)) {}

    char c;

    constexpr unsigned get_indexed_color_index() const {
        return indexed_color_index;
    }
    constexpr Color get_true_color() const { return true_color; }

    void set_true_color(Color const &value) {
        true_color = value;
        indexed_color_index = color_to_indexed(value);
    }

   private:
    static constexpr Color const indexed_colors[] = {
#ifdef _MSC_BUILD
        // windows console colors
        Color::rgb255(12, 12, 12),
        Color::rgb255(197, 15, 31),
        Color::rgb255(19, 161, 14),
        Color::rgb255(193, 156, 0),
        Color::rgb255(0, 55, 218),
        Color::rgb255(136, 23, 152),
        Color::rgb255(58, 150, 221),
        Color::rgb255(204, 204, 204),
#else
        // xterm colors
        Color::rgb24(0, 0, 0),
        Color::rgb24(205, 0, 0),
        Color::rgb24(0, 205, 0),
        Color::rgb24(205, 205, 0),
        Color::rgb24(0, 0, 238),
        Color::rgb24(205, 0, 205),
        Color::rgb24(0, 205, 205),
        Color::rgb24(229, 229, 229),
#endif
    };

    Color true_color;
    unsigned indexed_color_index;

    static constexpr unsigned color_to_indexed(Color const &color) {
        return std::min_element(
                   std::begin(indexed_colors),
                   std::end(indexed_colors),
                   [&color](Color const &a, Color const &b) {
                       return (a - color).get_sqr_magnitude() <=
                              (b - color).get_sqr_magnitude();
                   }
               ) -
               indexed_colors;
    }
};

#endif
