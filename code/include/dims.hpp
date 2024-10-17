#ifndef DIMS_HPP
#define DIMS_HPP

#include <cstdint>
#include <type_traits>

struct Size {
    Size() = delete;
    Size(uint16_t const w, uint16_t const h) : w(w), h(h) {}

    unsigned get_area() const { return w * h; }

    template <typename T>
    Size operator*(T const x) {
        static_assert(
            std::is_arithmetic<T>(),
            "Operator `*` does not exist for this type."
        );
        return Size(w * x, h * x);
    }

    template <typename T>
    Size operator/(T const x) {
        static_assert(
            std::is_arithmetic<T>(),
            "Operator `/` does not exist for this type."
        );
        return Size(w / x, h / x);
    }

    friend Size operator/(Size const &a, Size const &b) {
        return Size(a.w / b.w, a.h / b.h);
    }

    uint16_t w, h;
};

struct Pos {
    Pos() = delete;
    Pos(uint16_t const x, uint16_t const y) : x(x), y(y) {}

    uint16_t const x, y;
};

#endif
