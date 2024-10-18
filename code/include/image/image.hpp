#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>

#include "dims.hpp"

class Image {
   public:
    struct Pixel {
        Pixel() = delete;
        Pixel(uint8_t const r, uint8_t const g, uint8_t const b)
        : r(r), g(g), b(b) {}
        Pixel(uint32_t const hex)
        // dont have to be &'d with 255 coz got truncated during storing into u8
        : r(hex), g(hex >> 8), b(hex >> 16) {}

        uint32_t get_hex() const { return r | g << 8 | b << 16; }

        uint8_t const r, g, b;
    };

    virtual ~Image() {};

    virtual Size get_size(void) const = 0;
    virtual Pixel get_pixel(Pos const pos) const = 0;
};

#endif
