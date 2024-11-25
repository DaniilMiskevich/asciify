#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>
#include <cstdint>

struct Color {
    constexpr Color() : r(0.0), g(0.0), b(0.0) {}
    constexpr Color(float const r, float const g, float const b)
    : r(r), g(g), b(b) {}
    constexpr Color(uint32_t const hex)
    : r(float((hex >> 16) & 0xFF) / 0xFF),
      g(float((hex >> 8) & 0xFF) / 0xFF),
      b(float((hex >> 0) & 0xFF) / 0xFF) {}

    static constexpr Color
    rgb24(uint8_t const r, uint8_t const g, uint8_t const b) {
        return Color(float(r) / 0xFF, float(g) / 0xFF, float(b) / 0xFF);
    }

    float r, g, b;

    constexpr uint8_t get_r8() const { return r * 0xFF; }
    constexpr uint8_t get_g8() const { return g * 0xFF; }
    constexpr uint8_t get_b8() const { return b * 0xFF; }

    constexpr float get_sum() const {
        return 0.3333 * r + 0.3333 * g + 0.3333 * b + 0.0001;
    }
    constexpr float get_luminance() const {
        return 0.2126 * r + 0.7152 * g + 0.0722 * b;
    }

    constexpr float get_sqr_magnitude() const { return r * r + g * g + b * b; }
    constexpr float get_magnitude() const { return sqrtf(get_sqr_magnitude()); }

    constexpr explicit operator uint32_t() const {
        return uint8_t(r * 0xFF) << 16 | uint8_t(g * 0xFF) << 8 |
               uint8_t(b * 0xFF);
    }

    constexpr Color operator+(Color const &other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }
    constexpr Color operator-(Color const &other) const {
        return Color(r - other.r, g - other.g, b - other.b);
    }
    template <typename T>
    constexpr Color operator*(T const &other) const {
        return Color(r * other, g * other, b * other);
    }
    template <typename T>
    constexpr Color operator/(T const &other) const {
        return Color(r / other, g / other, b / other);
    }

    constexpr Color const &operator+=(Color const &other) {
        this->r += other.r;
        this->g += other.g;
        this->b += other.b;
        return *this;
    }
    constexpr Color const &operator-=(Color const &other) {
        this->r -= other.r;
        this->g -= other.g;
        this->b -= other.b;
        return *this;
    }
    template <typename T>
    constexpr Color const &operator*=(T const &other) {
        this->r *= other;
        this->g *= other;
        this->b *= other;
        return *this;
    }
    template <typename T>
    constexpr Color const &operator/=(T const &other) {
        this->r /= other;
        this->g /= other;
        this->b /= other;
        return *this;
    }
};

#endif
