#ifndef DIMS_HPP
#define DIMS_HPP

#include <cassert>
#include <cstdint>

struct Size {
    Size() = delete;

    constexpr Size(uint16_t const w, uint16_t const h) : w(w), h(h) {}

    uint16_t w, h;

    constexpr unsigned area() const { return w * h; }

    constexpr Size operator+(Size const &other) const {
        return Size(w + other.w, h + other.h);
    }
    constexpr Size operator-(Size const &other) const {
        return Size(w - other.w, h - other.h);
    }
    constexpr Size operator*(Size const &other) const {
        return Size(w * other.w, h * other.h);
    }
    constexpr Size operator/(Size const &other) const {
        return Size(w / other.w, h / other.h);
    }
    template <typename T>
    constexpr Size operator*(T const &other) const {
        return Size(w * other, h * other);
    }
    template <typename T>
    constexpr Size operator/(T const &other) const {
        return Size(w / other, h / other);
    }
};

struct Pos {
    Pos() = delete;

    constexpr Pos(uint16_t const xy) : x(xy), y(xy) {}
    constexpr Pos(uint16_t const x, uint16_t const y) : x(x), y(y) {}

    uint16_t x, y;

    constexpr bool operator==(Pos const &other) const {
        return x == other.x && y == other.y;
    }
    constexpr bool operator!=(Pos const &other) const {
        return !(*this == other);
    }

    constexpr Pos operator+(Pos const &other) const {
        return Pos(x + other.x, y + other.y);
    }
    constexpr Pos operator-(Pos const &other) const {
        return Pos(x - other.x, y - other.y);
    }
    constexpr Pos operator*(Size const &other) const {
        return Pos(x * other.w, y * other.h);
    }
    template <typename T>
    constexpr Pos operator*(T const &other) const {
        return Pos(x * other, y * other);
    }
    template <typename T>
    constexpr Pos operator/(T const &other) const {
        return Pos(x / other, y / other);
    }
};

#endif
