#ifndef DIMS_HPP
#define DIMS_HPP

#include <cstdint>
#include <type_traits>

struct Size {
    Size() = delete;
    Size(uint16_t const w, uint16_t const h) : w(w), h(h) {}

    unsigned get_area() const { return w * h; }

    template <typename T>
    friend Size operator/(Size const a, T const b) {
        static_assert(
            std::is_arithmetic<T>(),
            "`Size` can only be divisable by arithmetic types."
        );
        return Size(a.w / b, a.h / b);
    }

    uint16_t w, h;
};

struct Pos {
    Pos() = delete;
    Pos(uint16_t const x, uint16_t const y) : x(x), y(y) {}

    uint16_t const x, y;
};

#endif
