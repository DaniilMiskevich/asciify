#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>
#include <cstdint>

#include "conviniences.hpp"

struct Color {
    Color() = delete;

    constexpr Color(uint8_t const r, uint8_t const g, uint8_t const b)
    : r(r), g(g), b(b) {}

    constexpr explicit Color(uint32_t const hex)
    // dont have to be &'d with 255 coz got truncated during storing into u8
    : r(hex >> 16), g(hex >> 8), b(hex >> 0) {}

    constexpr uint8_t get_luminance() const {
        return 0.2126 * r + 0.7152 * g + 0.0722 * b;
    }

    constexpr static double distance(Color const &a, Color const &b) {
        let d = a - b;
        return (d.r * d.r + d.g * d.g + d.b * d.b);
    }

    constexpr explicit operator uint32_t() const {
        return r | g << 8 | b << 16;
    }

    constexpr Color operator-(Color const &other) const {
        double const a[3] = {r / 255.0, g / 255.0, b / 255.0};
        double const b[3] = {other.r / 255.0, other.g / 255.0, other.b / 255.0};
        return Color(
            fabs(a[0] - b[0]) * 255.0,
            fabs(a[1] - b[1]) * 255.0,
            fabs(a[2] - b[2]) * 255.0
        );
    }

    uint8_t const r, g, b;
};

#endif
