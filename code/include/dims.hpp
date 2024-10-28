#ifndef DIMS_HPP
#define DIMS_HPP

#include <cstdint>
#include <type_traits>

struct Size {
    Size() = delete;

    constexpr Size(uint16_t const w, uint16_t const h) : w(w), h(h) {}

    constexpr unsigned get_area() const { return w * h; }

    template <typename T>
    constexpr Size operator*(T const x) {
        static_assert(
            std::is_arithmetic<T>(),
            "Operator `*` does not exist for this type."
        );
        return Size(w * x, h * x);
    }

    template <typename T>
    constexpr Size operator/(T const &x) {
        static_assert(
            std::is_arithmetic<T>(),
            "Operator `/` does not exist for this type."
        );
        return Size(w / x, h / x);
    }

    constexpr Size operator/(Size const &b) { return Size(w / b.w, h / b.h); }

    uint16_t const w, h;
};

struct Pos {
    Pos() = delete;

    constexpr Pos(uint16_t const x, uint16_t const y) : x(x), y(y) {}

    uint16_t const x, y;
};

#endif
